import random
import os
import copy
class MonitorSettings:
    def __init__(self):
        '''
        mode : {nobody, allusers, newusers }
        '''
        self.secretpath = '/var/lib/kscreensystemsettings'

    def saveResolution(self, settings, identifier):
        globalSettings = copy.deepcopy(objects['VariablesManager'].get_variable('MONITORSETTINGS'))
        if globalSettings is None: 
            globalSettings = {'mode':None,'configurations':{}}
            globalSettings['configurations'][identifier] = settings
            objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['configurations'][identifier] = settings
            objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)
        if not os.path.exists(self.secretpath):
            newhash = "%032x"%random.getrandbits(256)
            with open(self.secretpath,'w') as fd:
                fd.write(newhash+"\n")
            os.chmod(self.secretpath,0o660)
        else:
            with open(self.secretpath,'r') as fd:
                newhash = fd.readline().strip()
        return {'status':True, 'msg': newhash}

    def updateResolution(self, settings, identifier, master_key):
        with open(self.secretpath,'r') as fd:
            localkey = fd.readline().strip()
        if localkey == master_key:
            return self.saveResolution(settings, identifier)
        else:
            return {'status':False,'msg':'Invalid master_key'}

    def saveMode(self, mode):
        globalSettings = copy.deepcopy(objects['VariablesManager'].get_variable('MONITORSETTINGS'))
        if globalSettings is None:
            globalSettings = {'mode': mode,'configurations':{}}
            objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['mode'] = mode
            objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)

    def getSettings(self):
        result = objects['VariablesManager'].get_variable('MONITORSETTINGS')
        if result is None:
            result = {'mode':'newusers','configurations':{}}
        return {'status':True, 'msg':result}

