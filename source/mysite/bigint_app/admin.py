from django.contrib import admin

from .models import div, div_rule

class divAdmin(admin.ModelAdmin):
	list_display = ('date','dividend','dividend_size','divisor','divisor_size','runtime','return_val')

class ruleAdmin(admin.ModelAdmin):
	list_display = ('date','divisor','divisor_size','rule','rule_size','negative_rule')

# Register your models here.

admin.site.register(div, divAdmin)
admin.site.register(div_rule, ruleAdmin)
