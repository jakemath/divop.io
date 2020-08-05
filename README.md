# Divisibility-Operator-API
A Django framework with SQLite backend and views returning divisibility data in JSON format to wrap Divisibility Operator C++ code. When the following endpoints are queried, the SQLite database is first searched for existence of relevant operand entries. If the arguments are not found, the computations are carried out.

To execute a div operation on random operands of specified size:

    https://(localhost).com/bigint_app/div/random/<dividend_size>/<divisor_size>

To execute a div operation on custom operands:

    https://(localhost).com/bigint_app/div/custom/<dividend>/<divisor>

Div operation results are returned in JSON format with the dividend, divisor, operand sizes, runtime in milliseconds, and return value.

To compute the rule for a randomly sized and valid divisor:

    https://(localhost).com/bigint_app/rule/random/<divisor_size>
    
To compute the rule for a custom divisor:

    https://(localhost).com/bigint_app/rule/custom/<divisor>
    
Rule computation results are also returned in JSON format with the divisor and divisor size, rule and rule size, and whether the rule is negative or not.
