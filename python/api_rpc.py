'''
Created on 10 aout 2016

@author: Sesa432188
'''

from flask import *
from flask.views import View
from werkzeug.datastructures import ImmutableMultiDict
import json

from services.facilityHero.measures.DateFormatter import DateFormatter

import logging
log = logging.getLogger(__name__)

class Eowok(object):
    
    def __init__(self,world="Endor"):
        self.world = world

        
app = Flask(__name__)
def control_type(*types):
    def decorateur(fonction):
        def parse_argument(*args):
            argument = []
            log.debug("types",types)
            log.debug("argument",args)
            for type_name,arg in zip(types,args):
                #call constructor of type_name with arg
                argument.append(type_name(arg))
            return fonction(*argument)
        return parse_argument
    return decorateur

@control_type(str,float,Eowok)
def hello(pseudo,number,eowok):
    c = number + 1
    return "salut " + pseudo + str(c) + " " +eowok.world

def buildResponse(result,uid,error="null"):
    obj = {}
    obj["result"] = result
    obj["id"] = uid
    obj["error"] = error
    return obj

class MethodRegistry(object):
    
    def __init__(self):
        self.registry = {}
        self.registry["display"] = self.display
        
    def register(self,method,method_name=None):
        if method_name is not None:
            self.registry[method_name]= method
        else:
            self.registry[method.__name__]= method
    
    """
    Affiche toutes les methodes 
    """
    def display(self):
        result = []
        for method in self.registry:
            print(method,self.registry[method].__code__.co_varnames)
        return ",".join(self.registry.keys())
            
    def execute(self,method,params=None,id=0):
        try:
            if params is not None:
                return buildResponse(self.registry[method](*params),id)
            return buildResponse(self.registry[method](),id)
        except KeyError:
            return buildResponse("null",id,"method not registered")
        
class RPC_manager(View):
    
    methods = ['GET', 'POST']
    
    def __init__(self,registry):
        self.registry = registry
         
    def conf(self):
        pass
    
    def register(self,method,method_name=None):
        self.registry.register(method,method_name)
    
    def dispatch_request(self,method):
        if request.method == "POST":
            log.debug("request param",request.get_json()["params"])
            params = request.get_json()["params"]
            try:
                method_name = request.get_json()["method"]
            except:
                return "method not given"
            uid = 0
            try:
                uid = request.get_json()["id"]
            except:
                pass
            result = self.registry.execute(method_name,params=params,id=uid)
            print(result)
        else:
            result = self.registry.execute(method)
            print(result)
        return str(result)
    
if __name__ == '__main__':
    #hello("bob")
    date = DateFormatter()
    registry = MethodRegistry()
    registry.register(hello,"hello")
    #je peux mettre toutes les methodes de l'instance date
    registry.register(date.getDate_day)
    registry.register(date.getDate_mounth)
    app.add_url_rule('/rpc/<method>',view_func=RPC_manager.as_view('api.facility.rpc',registry),methods = ['GET','POST'])
    app.run(debug=True)