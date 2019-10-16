import random
import os
class MonitorSettings:
    def __init__(self):
        self.secretpath = '/var/lib/kscreensystemsettings'

    def saveResolution(self, settings, identifier):
        globalSettings = objects['VariablesManager'].get_variable('MONITORSETTINGS')
        if globalSettings is None: 
            globalSettings = {'mode':None,'configurations':{}}
            globalSettings['configurations'][identifier] = settings
            objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['configurations'][identifier] = settings
            objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)
        newhash = "%032x"%random.getrandbits(256)
        with open(self.secretpath,'w') as fd:
            fd.write(newhash "\n")
        os.chmod(self.secretpath,0o660)
        return {'status':True, 'msg': newhash}

    def updateResolution(self, settings, identifier, master_key):
        with open(self.secretpath,'r') as fd:
            localkey = fd.readline().strip()
        if localkey == master_key:
            return self.saveResolution(settings, identifier)
        else:
            return {'status':False,'msg':'Invalid master_key'}

    def saveMode(self, mode):
        globalSettings = objects['VariablesManager'].get_variable('MONITORSETTINGS')
        if globalSettings is None:
            globalSettings = {'mode': mode,'configurations':{}}
            objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['mode'] = mode
            objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)

    def getSettings(self):
        result = objects['VariablesManager'].get_variable('MONITORSETTINGS')
        if result is None:
            result = {'mode':None,'configurations':{}}
        return result

