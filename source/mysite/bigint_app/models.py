from django.db import models

# Create your models here.

class div(models.Model):
	date = models.DateTimeField()
	dividend = models.TextField()
	dividend_size = models.BigIntegerField()
	divisor = models.TextField()
	divisor_size = models.BigIntegerField()
	runtime = models.BigIntegerField()
	return_val = models.BooleanField()
	class Meta:
		unique_together = (("dividend", "divisor"),)
	def __str__(self):
		s = self.dividend + ' div ' + self.divisor + ' = '
		s += str(self.return_val)
		return s

class div_rule(models.Model):
    divisor = models.TextField(primary_key=True)
    divisor_size = models.BigIntegerField()
    rule = models.TextField()
    rule_size = models.BigIntegerField()
    negative_rule = models.BooleanField()
    date = models.DateTimeField(default=None)
    def __str__(self):
        return self.divisor + ' --> ' + self.rule
		
