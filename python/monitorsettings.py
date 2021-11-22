import random
import os
import copy
class MonitorSettings:
    def __init__(self):
        '''
        mode : {nobody, allusers, newusers }
        '''
        self.secretpath = '/var/lib/kscreensystemsettings'
        self.filetype_map = {"config": "MONITORSETTINGS", "outputs": "MONITOROUTPUTS", "control" : "MONITORCONTROL"}

    def saveResolution(self, settings, identifier, filetype):
        if filetype not in self.filetype_map:
            return {"status": False, "msg":"Wrong Filetype" }
        n4d_filetype_var = self.filetype_map[ filetype ]

        globalSettings = copy.deepcopy(objects['VariablesManager'].get_variable(n4d_filetype_var))
        if globalSettings is None: 
            globalSettings = {'configurations':{}}
            if filetype == "config":
                globalSettings["mode"] = None
            globalSettings['configurations'][identifier] = settings
            objects['VariablesManager'].add_variable(n4d_filetype_var, globalSettings, {}, '', '')
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

    def updateResolution(self, settings, identifier, master_key, filetype):
        with open(self.secretpath,'r') as fd:
            localkey = fd.readline().strip()
        if localkey == master_key:
            return self.saveResolution(settings, identifier, filetype)
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

    def getSettings(self, filetype):
        result = objects['VariablesManager'].get_variable(self.filetype_map[filetype])
        if result is None:
            result = {'configurations':{}}
            if filetype == "config":
                result["mode"] = "nobody" 
        return {'status':True, 'msg':result}

