# divop.io

Have you ever needed to divide million-digit integers, but didn't have the time nor the paper required to do it? Well, you're in luck!

divop.io is a fast REST/WebSocket API suite built on the Python FastAPI framework for computing divisibility on arbitrarily large numbers.

Uses a C++ implementation of the general divisiblity criteria with capacity to compute on ridiculously large numbers containing 1M+ digits. 

The divisiblity algorithm leverages the generalization of integer divisibility rules for all prime numbers. This algorithm has a runtime complexity of `O(nm)`, where `n` is the digits in the dividend and `m` is the digits in the divisor.

[Runtime analysis](./Runtimes.pdf)

## Setup
Clone the repo
```bash
cd ~/
git clone https://github.com/jakemath/divop.io
```
Ensure you have `docker` and `docker-compose` installed on your machine. Start the web server:
```bash
cd divop.io
bash run.sh
```

The API suite exposes a couple of useful and/or interesting endpoints for computing.

## REST Endpoints: `http://divop.io/`
### Uniform Random Number Generator `/generate/{size}`
Iteratively generates a random number with `size` digits. The digits are uniformly distributed over the interval `[0,...,9]`

#### Example Request
```bash
curl http://divop.io/generate/11
```

#### Example Output
```json
{
    "response": "12312412412",
    "runtime_ms": 0.01
}
```

### Divisibility `/div/{dividend}/{divisor}`
Executes the divisibility algorithm on the specified `dividend` and `divisor`. Returns `true` if `divisor` divides `dividend`, else `false`

#### Example Request
```bash
curl http://divop.io/div/111/3
```

#### Example Output
```json
{
    "result": true,
    "runtime_ms": 0.01
}
```

### Random Divisiblity `/rand-div/{dividend_size}/{divisor_size}`
Executes the divisibility algorithm on randomly generated dividend and divisor operands of size `dividend_size` and `divisor_size` digits, respectively. Numbers are generated according to the same procedure as the random number generator endpoint. 

Returns `true` if the random divisor divides the random dividend, else false.

#### Example Request
```
curl http://divop.io/rand-div/111/3
```

#### Example Output
```json
{
    "result": true,
    "dividend": "{some 111 digit number}",
    "divisor": "{some 3 digit number}",
    "dividend_size": 111,
    "divisor_size": 3,
    "runtime_ms": 0.01
}
```

## WebSocket Endpoints `ws://divop.io/`
For calculations on gargantuan operands, the WebSocket endpoint may prove useful in avoiding request timeouts. This is best suited for calculations involving numbers containing 1M+ digits, but please don't test higher than that. 

### Uniform Random Number Generator `/ws/generate`
Iteratively generates a random number with `size` digits. The digits are uniformly distributed over the interval `[0,...,9]`

#### Example Request Payload
```json
{
    "size": 10000
}
```

#### Example Output
```json
{
    "response": "{some 10000 digit number}",
    "runtime_ms": 0.01
}
```

### Divisibility `/ws/div`
Executes the divisibility algorithm on the specified `dividend` and `divisor`. Returns `true` if `divisor` divides `dividend`, else `false`

#### Example Request Payload
```json
{
    "dividend": "{some giant number}",
    "divisor": "{some giant number}"
}
```

#### Example Output
```json
{
    "response": true,
    "runtime_ms": 0.01
}
```

### Random Divisiblity `/ws/rand-div`
Executes the divisibility algorithm on randomly generated dividend and divisor operands of size `dividend_size` and `divisor_size` digits, respectively. Numbers are generated according to the same procedure as the random number generator endpoint. 

Returns `true` if the random divisor divides the random dividend, else false.

#### Example Request Payload
```json
{
    "dividend_size": 1000,
    "divisor_size": 3
}
```

#### Example Output
```json
{
    "result": true,
    "dividend": "{some 1000 digit number}",
    "divisor": "{some 3 digit number}",
    "dividend_size": 1000,
    "divisor_size": 3,
    "runtime_ms": 0.01
}
```
