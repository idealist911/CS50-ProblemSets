# THIS PROGRAM ASKS USER FOR A CREDIT CARD NUMBER
# AND OUTPUTS THE CREDIT CARD COMPANY


def main():
    # Get user input
    card = input("Number: ")
    while (card.isdigit() == False or int(card) <= 0):
        card = input("Number: ")

    # get first two char in card
    card_start = int(card[:2])
    card_num = int(card)
    card_length = len(card)

    card = {
        'type': 'INVALID',
        'number': card_num,
        'length': card_length,
        'start': card_start
    }

    # card types
    amex = {
        'type': 'AMEX',
        'length': [15],
        'start': [34, 37]
    }

    master = {
        'type': 'MASTERCARD',
        'length': [16],
        'start': range(51, 56, 1)
    }

    visa = {
        'type': 'VISA',
        'length': [13, 16],
        'start': range(40, 50, 1)
    }
    types = [amex, master, visa]

    # sets status as invalid first
    # turns valid when match length and start
    invalid = 1

    # match length and start with different types
    for co in types:
        for typelength in co['length']:
            if (card['length'] == typelength):
                for typestart in co['start']:
                    if (card['start'] == typestart):
                        invalid = 0
                        card['type'] = co['type']
                        break

    # only check the checksum if length and start
    # matches one of the types
    if (not invalid):
        checksum = get_checksum(card)

        # failure criterion
        failed = checksum % 10

        # invalidate if failed checksum
        if (failed):
            invalid = 1
            card['type'] = 'INVALID'

    print(card['type'])


# Get checksum of credit card using Luhn's algorithm
def get_checksum(card):
    oddsum = 0
    evensum = 0
    card_num = card['number']
    for i in range(card['length']):
        digit = card_num % 10
        card_num = int(card_num / 10)

        # different operations for odd and even powers of 10
        if (i % 2):
            digit *= 2
            if (digit > 9):
                digit1 = int(digit % 10)
                digit10 = int(digit / 10)
                oddsum += (digit1 + digit10)
            else:
                oddsum += digit
        else:
            evensum += digit

    checksum = oddsum + evensum
    # print(checksum)

    return checksum


# EXECUTE MAIN
if __name__ == '__main__':
    main()