import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

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

    stocks = db.execute("SELECT symbol FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=session["user_id"])
    total = 0

    for i in range(len(stocks)):
        stocks[i]['shares'] = db.execute("SELECT sum(shares) FROM transactions WHERE user_id=:user_id AND symbol = :symbol",
                                         user_id=session["user_id"], symbol=stocks[i]['symbol'])[0]['sum(shares)']
        stocks[i]['name'] = lookup(stocks[i]['symbol'])['name']
        stocks[i]['price'] = lookup(stocks[i]['symbol'])['price']
        total = total + (stocks[i]['shares'] * stocks[i]['price'])

    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session['user_id'])[0]['cash']

    total = usd(total + cash)

    cash = usd(cash)

    return render_template("index.html", stocks=stocks, total=total, cash=cash)


@app.route("/addfunds", methods=["GET", "POST"])
@login_required
def addfunds():
    """Let the user add funds"""
    if request.method == "POST":

        amount = int(request.form.get("amount"))
        if amount == None:
            return apology("Must enter amount", 400)
        else:
            cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
            balance = cash + amount
            db.execute("UPDATE users SET cash = :balance WHERE id = :user_id", balance=balance, user_id=session["user_id"])

            return redirect("/")
    else:
        return render_template("addfunds.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

        if quote == None:
            return apology("must enter valid symbol", 400)
        elif request.form.get("shares") == "":
            return apology("must enter number of shares", 400)

        shares = int(request.form.get("shares"))
        if quote["price"] * shares > cash:
            return apology("not enough funds", 400)

        else:

            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)", user_id=session["user_id"], symbol=symbol,
                       shares=shares, price=quote["price"], time=datetime.now())

            balance = cash - (shares * quote["price"])

            db.execute("UPDATE users SET cash = :balance WHERE id = :user_id", balance=balance, user_id=session["user_id"])

            return redirect("/")

    if request.method == "GET":
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute(
        "SELECT symbol, shares, price, time FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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

        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        if quote == None:
            return apology("must enter valid symbol")
        else:
            return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    # User reached route via POST
    if request.method == "POST":

        # Ensure user types username
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure user types password
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure user confirms password
        elif not request.form.get("password2"):
            return apology("must confirm password", 400)

        # Ensure confirmation matches password
        elif not request.form.get("password") == request.form.get("password2"):
            return apology("password confirmation does not match", 400)

        # Store username inside variable
        username = request.form.get("username")

        # Ensure username is not already taken
        duplicate = db.execute("SELECT * FROM users WHERE username = :username", username=username)
        if duplicate:
            return apology("username already exists", 400)

        # Store password and hash inside variables
        password = request.form.get("password")
        hashpass = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # Insert username and hash into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashpass)", username=username, hashpass=hashpass)
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    stocks = db.execute("SELECT symbol FROM transactions WHERE user_id=:user_id GROUP BY symbol;", user_id=session["user_id"])

    for i in range(len(stocks)):
        stocks[i]['shares'] = db.execute("SELECT sum(shares) FROM transactions WHERE user_id=:user_id AND symbol = :symbol",
                                         user_id=session["user_id"], symbol=stocks[i]['symbol'])[0]['sum(shares)']

    if request.method == "POST":

        symbol = request.form.get("symbol")
        if symbol == None:
            return apology("Must provide symbol", 400)
        for i in range(len(stocks)):
            if symbol == stocks[i]['symbol']:
                shares_owned = stocks[i]['shares']
        if request.form.get("shares") == "":
            return apology("must enter number of shares", 400)

        shares = -int(request.form.get("shares"))
        if -shares > shares_owned:
            return apology("Not enough shares", 400)
        else:
            cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
            quote = lookup(symbol)
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)", user_id=session["user_id"], symbol=symbol,
                       shares=shares, price=quote["price"], time=datetime.now())

            balance = cash - (shares * quote["price"])

            db.execute("UPDATE users SET cash = :balance WHERE id = :user_id", balance=balance, user_id=session["user_id"])

            return redirect("/")
    else:
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
