from django.urls import path
from . import views

urlpatterns = [
	path('', views.index, name='index'),
	path('div/random/<str:dividend_size>/<str:divisor_size>', views.div_random, name='div_random'),
	path('div/custom/<str:dividend>/<str:divisor>', views.div_custom, name='div_custom'),
   	path('rule/custom/<str:divisor_>', views.rule_custom, name='rule_custom'),
    	path('rule/random/<str:divisor_size>', views.rule_random, name='rule_random')
]
