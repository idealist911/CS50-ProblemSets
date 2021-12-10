import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():

    if request.method == "POST":
        errorMsg = None

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        if not name:
            errorMsg = "Missing name";

        month = request.form.get("month")
        if not month:
            errorMsg = "Missing month";
        try:
            int(month)
            if int(month) > 12:
                errorMsg = "Month should be lesser than 12.";
            if int(month) < 1:
                errorMsg = "Month should be greater than 0.";
        except ValueError:
            errorMsg = "Month should be an integer.";

        day = request.form.get("day")
        if not day:
            errorMsg = "Missing day";
        try:
            int(day)
            if int(day) > 31:
                errorMsg = "The longest months have only 31 days.";
            if int(day) < 1:
                errorMsg = "Day should be greater than 0.";
        except ValueError:
            errorMsg = "Day should be an integer.";

        if not(errorMsg):
            if int(month) == 2 and int(day) > 29:
                errorMsg = "February has no more than 29 days, even on a leap year.";

        if errorMsg:
            birthdays = db.execute("SELECT * FROM birthdays")
            return render_template("index.html", birthdays=birthdays, errorMsg=errorMsg)


        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, int(month), int(day))

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")

        # return render_template("index.html")
        return render_template("index.html", errorMsg=None, birthdays=birthdays)


@app.route("/delete", methods=["POST"])
def delete():
    if request.method == "POST":
        id = request.form.get("id")
        db.execute("DELETE FROM birthdays WHERE id=(?)", id)
        return redirect("/")

    else:
        return redirect("/")

