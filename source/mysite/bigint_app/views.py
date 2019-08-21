from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from django.utils import timezone
from django.template import loader
from django.core.exceptions import ObjectDoesNotExist
from bigint_app.models import div, div_rule
import random
import string
import os
import subprocess
# Create your views here.

dir = # INSERT ABSOLUTE PATH TO BIGINT_CODE DIRECTORY #

# Random string generator for naming temp files
def randstr():
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(6))

# Default index page returns all items in div_rule table with rule_size < 300
def index(request):
    context = { 'div_rules': div_rule.objects.filter(rule_size__lt=300) }
    return HttpResponse(loader.get_template('bigint_app/index.html').render(context,request))

# Generate random operands and execute div operation
def div_random(request, dividend_size, divisor_size):
    if dividend_size.isdigit() == 0 
    or divisor_size.isdigit() == 0 
    or int(dividend_size) <= 0 
    or int(divisor_size) <= 0:
        return HttpResponse('Invalid input')
    os.chdir(dir)
    operands_file = randstr() + '.txt' # Write operands to file
    os.system('./generate_operands ' + dividend_size + ' ' + divisor_size + ' > ' + operands_file)
    operands = open(operands_file).readlines()[0].split(',') # Parse operands in file
    r = get_or_create_rule(operands[0], operands_file) # Find or create div rule for divisor operand
    div = get_or_create_div(operands[1], operands[0], operands_file) # Execute div operation
    os.system('rm ' + operands_file) # Delete temp file
    data = { # Return JSON formatted div results
        'date' : timezone.now(),
        'dividend' : div.dividend,
        'dividend_size' : len(div.dividend),
        'divisor' : div.divisor,
        'divisor_size' : len(div.divisor),
        'return_val' : div.return_val,
        'runtime' : div.runtime
    }
    return JsonResponse(data)

# Div operation on custom operands
def div_custom(request, dividend_, divisor_):
    if dividend_.isdigit() == 0 
    or divisor_.isdigit() == 0 
    or (int(divisor_[-1]) % 2 == 0 and divisor_ is not '2') 
    or (divisor_[-1] is '5' and divisor_ is not '5'):
        return HttpResponse('Invalid input')
    os.chdir(dir)
    operands_file = randstr() + '.txt'
    open(operands_file, 'w').write(divisor_ + ',' + dividend_) # Write operands to temp file
    r = get_or_create_rule(divisor_, operands_file_) # Find or create div rule for divisor
    test = get_or_create_div(dividend_, divisor_, operands_file) # Find or create div result for operands
    os.system('rm ' + operands_file) # Delete temp file
    data = { # Return JSON formatted div results
        'date' : test.date,
        'dividend' : test.dividend,
        'dividend_size' : test.dividend_size,
        'divisor' : test.divisor,
        'divisor_size' : test.divisor_size,
        'runtime' : test.runtime,
        'return_val' : test.return_val
    }
    return JsonResponse(data)

# Generate random divisor and compute corresponding rule
def rule_random(request, divisor_size):
    if divisor_size.isdigit() == 0 or int(divisor_size) <= 0:
        return HttpResponse('Invalid input')
    os.chdir(dir)
    divisor_file = randstr() + '.txt'
    os.system('./generate_operand ' + divisor_size + ' > ' + divisor_file) # Write rule to temp file
    divisor_ = open(divisor_file).readlines()[0]
    test = get_or_create_rule(divisor_, divisor_file) # Generate rule for operand
    os.system('rm ' + divisor_file) # Delete temp file
    data = { # Return JSON formatted data
        'date' : test.date,
        'divisor' : test.divisor,
        'divisor_size' : test.divisor_size,
        'rule' : test.rule,
        'rule_size' : test.rule_size,
        'negative_rule' : test.negative_rule
    }
    return JsonResponse(data)

# Generate rule for a custom operand
def rule_custom(request, divisor_):
    if divisor_.isdigit() == 0 
    or int(divisor_) <= 0 
    or int(divisor_[-1]) % 2 == 0 
    or divisor_[-1] == '5' 
    or divisor_ in ['1','2','3','5']:
        return HttpResponse('Invalid input')
    os.chdir(dir)
    divisor_file = randstr() + '.txt'
    open(divisor_file, 'w').write(divisor_) # Write divisor to file
    rule_ = get_or_create_rule(divisor_, divisor_file) # Compute rule
    os.system('rm ' + divisor_file) # Delete temp file
    data = { # Return JSON formatted data
        'date' : rule_.date,
        'divisor' : rule_.divisor,
        'divisor_size' : rule_.divisor_size,
        'rule' : rule_.rule,
        'rule_size' : rule_.rule_size,
        'negative_rule' : rule_.negative_rule
    }
    return JsonResponse(data)

# Check if dividend_ and divisor_ present in div table, else compute and insert
def get_or_create_div(dividend_, divisor_, operands_file):
    try:
        div_ = div.objects.get(dividend=dividend_, divisor=divisor_)
    except ObjectDoesNotExist:
        results = subprocess.Popen( # Capture stdout
            ['./div_custom', operands_file],
            stdout=subprocess.PIPE,
            universal_newlines=True).communicate()[0].split(',')
        div_ = div.objects.create( # Insert into div table
            date=timezone.now(),
            dividend=dividend_,
            dividend_size=len(dividend_),
            divisor=divisor_,
            divisor_size=len(divisor_),
            return_val=results[0],
            runtime=results[1]
        )
        div_.save()
    return div_

# Check if divisor_ present in div_rule table, else compute rule and insert
def get_or_create_rule(divisor_, operands_file):
    if divisor_ not in ['1','2','3','5']: # No div rule for 1, 2, 3 or 5
        try: # Check if exists
            rule_ = div_rule.objects.get(divisor=divisor_)
        except ObjectDoesNotExist: # If does not exist
            rule = subprocess.Popen( # Capture stdout
                ['./rule_custom', operands_file],
                stdout=subprocess.PIPE,
                universal_newlines=True).communicate()[0]
            if rule[0] == '-': # Adjust rule size for negative rule
                length = len(rule) - 1
            else:
                length = len(rule)
            rule_ = div_rule.objects.create( # Return JSON formatted rule results
                divisor=divisor_,
                divisor_size=len(divisor_),
                rule=rule,
                rule_size=length,
                negative_rule=(rule[0] is '-'),
                date=timezone.now()
            )
            rule_.save()
        return rule_
