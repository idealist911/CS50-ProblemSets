import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime # to store dates on which transactions happen

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = rows[0]["cash"]

    # Initialize the total value of account: cash (stocks to be added later)
    total_value = cash

    # If transactions table has not been created, e.g. for first user
    if not (db.execute("SELECT * FROM SQLITE_MASTER WHERE TYPE='table' AND NAME='transactions'")):
        return render_template("index.html", stocks={}, cash=cash, total_value=total_value)

    # If user has not sold any stocks, collate records from buy segment of transactions table
    if (db.execute("SELECT CASE WHEN EXISTS(SELECT 1 FROM transactions WHERE type = 'sell') THEN 1 ELSE 0 END AS state")[0]['state'] == 0):

        stocks = db.execute(
            "SELECT symbol, SUM(num_share) AS total_shares \
            FROM transactions \
            WHERE type = 'buy' \
            AND user_id = ? \
            GROUP BY symbol \
            HAVING total_shares > 0"
            , session["user_id"])

    # If user has sold at least one stock
    else:

        stocks = db.execute(
            "SELECT A.symbol AS symbol, \
            CASE \
                WHEN B.total_share IS NULL THEN A.total_share \
                ELSE (A.total_share - B.total_share) \
                END AS total_shares \
            FROM \
                (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'buy' GROUP BY symbol) A \
                LEFT JOIN \
                (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'sell' GROUP BY symbol) B \
                ON A.symbol = B.symbol\
            WHERE A.user_id = ? \
            AND total_shares > 0 \
            GROUP BY A.symbol \
            "
            , session["user_id"])

    for stock in stocks:
        stock["current_price"] = lookup(stock["symbol"])["price"]
        stock["value"] = stock["total_shares"] * stock["current_price"]
        stock["name"] = lookup(stock["symbol"])["name"]
        total_value = total_value + stock["value"]

    return render_template("index.html", stocks=stocks, cash=cash, total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))

        if not stock:
            return apology("invalid symbol", 400)

        numshares = request.form.get("shares")

        # check for integer strings, i.e. can be turned into an int
        try:
            numshares = int(numshares)
            # if integer, make sure it is larger than 1
            if numshares < 1:
                return apology("invalid shares", 400)
        except ValueError:
            return apology("invalid shares", 400)

        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        if cash < stock["price"] * numshares:
            return apology("not enough cash", 403)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - stock["price"] * numshares, session["user_id"])

        # update/create transaction table
        user_id = session["user_id"]
        symbol = stock["symbol"]
        price = stock["price"]
        rows = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])

        # if transactions table not created yet, create it
        if not (db.execute("SELECT * FROM SQLITE_MASTER WHERE TYPE='table' AND NAME='transactions'")):
            db.execute(
                "CREATE TABLE transactions (\
                    time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
                    type    TEXT,\
                    user_id  INTEGER,\
                    symbol TEXT,\
                    num_share   INTEGER,\
                    price_per_share FLOAT,\
                    FOREIGN KEY(user_id) REFERENCES users(id))"
                )

        # insert new transaction entry
        db.execute("INSERT INTO transactions (type, user_id, symbol, num_share, price_per_share) VALUES (?,?,?,?,?)", "buy", user_id, symbol, numshares, price)

        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # If transactions table has not been created, e.g. for first user
    if not (db.execute("SELECT * FROM SQLITE_MASTER WHERE TYPE='table' AND NAME='transactions'")):
        return render_template("history.html", stocks={})

    stocks = db.execute(
        "SELECT *, DATE(time) as date, TIME(time) as time \
        FROM transactions \
        WHERE user_id = ? \
        "
        , session["user_id"])

    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # if user did not provide any symbol
        if not request.form.get("symbol"):
            return apology("invalid symbol", 400)

        # if user provided invalid symbol
        if not lookup(request.form.get("symbol")):
            return apology("invalid symbol", 400)

        return render_template("quoted.html",stock=lookup(request.form.get("symbol")))

    # User reached route by following link
    else:
        # Redirect user to login form
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (i.e. by submitting a form)
    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("confirmation does not match password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exist
        if len(rows) >= 1:
            return apology("username already exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/login")

    # User reached route via GET (i.e. by clicking a link or redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        num_shares = int(request.form.get("shares"))

        # If user has not sold any stocks, collate records from buy segment of transactions table
        if (db.execute("SELECT CASE WHEN EXISTS(SELECT 1 FROM transactions WHERE type = 'sell') THEN 1 ELSE 0 END AS state")[0]['state'] == 0):

            stocks = db.execute(
                "SELECT \
                SUM(num_share) AS total_shares \
                FROM transactions \
                WHERE type = 'buy' \
                AND user_id = ? \
                AND symbol = ? \
                GROUP BY symbol \
                HAVING total_shares > 0"
                , session["user_id"], stock["symbol"]
                )

        # If user has sold at least one stock
        else:

            stocks = db.execute(
                "SELECT A.symbol AS symbol, \
                CASE \
                    WHEN B.total_share IS NULL THEN A.total_share \
                    ELSE (A.total_share - B.total_share) \
                    END AS total_shares \
                FROM \
                    (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'buy' GROUP BY symbol) A \
                    LEFT JOIN \
                    (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'sell' GROUP BY symbol) B \
                    ON A.symbol = B.symbol\
                WHERE A.user_id = ? \
                AND total_shares > 0 \
                GROUP BY A.symbol \
                "
                , session["user_id"])

        if num_shares > stocks[0]["total_shares"]:
            return apology("Not enough shares", 400)


        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + stock["price"] * num_shares, session["user_id"])

        date = datetime.datetime.now()
        db.execute("INSERT INTO transactions (type, user_id, symbol, num_share, price_per_share) VALUES (?,?,?,?,?)", "sell", session["user_id"], stock["symbol"], num_shares, stock["price"])

        return redirect("/")

    # If transactions table has not been created, e.g. for first user
    if not (db.execute("SELECT * FROM SQLITE_MASTER WHERE TYPE='table' AND NAME='transactions'")):
        return render_template("sell.html", stocks={})

    # If user has not sold any stocks, collate records from buy segment of transactions table
    if (db.execute("SELECT CASE WHEN EXISTS(SELECT 1 FROM transactions WHERE type = 'sell') THEN 1 ELSE 0 END AS state")[0]['state'] == 0):

        stocks = db.execute(
            "SELECT \
            symbol, SUM(num_share) AS total_shares \
            FROM transactions \
            WHERE type = 'buy' \
            AND user_id = ? \
            GROUP BY symbol \
            HAVING total_shares > 0"
            , session["user_id"]
            )

    # If user has sold at least one stock
    else:

        stocks = db.execute(
            "SELECT A.symbol AS symbol, \
            CASE \
                WHEN B.total_share IS NULL THEN A.total_share \
                ELSE (A.total_share - B.total_share) \
                END AS total_shares \
            FROM \
                (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'buy' GROUP BY symbol) A \
                LEFT JOIN \
                (SELECT *, SUM(num_share) AS total_share FROM transactions WHERE type = 'sell' GROUP BY symbol) B \
                ON A.symbol = B.symbol\
            WHERE A.user_id = ? \
            AND total_shares > 0 \
            GROUP BY A.symbol \
            "
            , session["user_id"])


    return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
