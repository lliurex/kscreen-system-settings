import random
import os
import n4d.server.core as n4dcore
import n4d.responses

class MonitorSettings:


    WRONG_FILETYPE = -50


    def __init__(self):
        '''
        mode : {nobody, allusers, newusers }
        '''
        self.secretpath = '/var/lib/kscreensystemsettings'
        self.core = n4dcore.Core.get_core()
        self.filetype_map = {"config": "MONITORSETTINGS", "outputs": "MONITOROUTPUTS", "control" : "MONITORCONTROL"}

    def saveResolution( self, settings, identifier, filetype ):
        #Old n4d: globalSettings = copy.deepcopy(objects['VariablesManager'].get_variable('MONITORSETTINGS'))
        if filetype not in self.filetype_map:
            return n4d.responses.build_failed_call_response( MonitorSettings.WRONG_FILETYPE )
        n4d_filetype_var = self.filetype_map[ filetype ]

        globalSettings = self.core.get_variable(n4d_filetype_var).get('return',None)
        if globalSettings is None: 
            globalSettings = {'configurations':{}}
            if filetype == "config":
                globalSettings["mode"] = None
            globalSettings['configurations'][identifier] = settings
            #Old n4d: objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
            self.core.set_variable(n4d_filetype_var,globalSettings)
        else:
            globalSettings['configurations'][identifier] = settings
            #Old n4d: objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)
            self.core.set_variable(n4d_filetype_var, globalSettings)

        if not os.path.exists(self.secretpath):
            newhash = "%032x"%random.getrandbits(256)
            with open(self.secretpath,'w') as fd:
                fd.write(newhash+"\n")
            os.chmod(self.secretpath,0o660)
        else:
            with open(self.secretpath,'r') as fd:
                newhash = fd.readline().strip()
        #Old n4d:return {'status':True, 'msg': newhash}
        return n4d.responses.build_successful_call_response(newhash)

    def updateResolution( self, settings, identifier, master_key, filetype ):
        with open( self.secretpath, 'r' ) as fd:
            localkey = fd.readline().strip()
        if localkey == master_key:
            return self.saveResolution(settings, identifier, filetype )
        else:
            #Old n4d:return {'status':False,'msg':'Invalid master_key'}
            return n4d.responses.build_failed_call_response()

    def saveMode(self, mode):
        #Old n4d: globalSettings = copy.deepcopy(objects['VariablesManager'].get_variable('MONITORSETTINGS'))
        globalSettings = self.core.get_variable('MONITORSETTINGS').get('return',None)

        if globalSettings is None:
            globalSettings = {'mode': mode,'configurations':{}}
            #Old n4d: objects['VariablesManager'].add_variable('MONITORSETTINGS', globalSettings, {}, '', '')
            self.core.set_variable('MONITORSETTINGS', globalSettings)

        else:
            globalSettings['mode'] = mode
            #Old n4d:objects['VariablesManager'].set_variable('MONITORSETTINGS', globalSettings)
            self.core.set_variable('MONITORSETTINGS', globalSettings)
        return n4d.responses.build_successful_call_response()


    def getSettings(self, filetype):
        #Old n4d:result = objects['VariablesManager'].get_variable('MONITORSETTINGS')
        result = self.core.get_variable(self.filetype_map[filetype]).get('return',None)

        if result is None:
            result = {'configurations':{}}
            if filetype == "config":
                result["mode"] = "nobody"
        #Old n4d: return {'status':True, 'msg':result}
        return n4d.responses.build_successful_call_response(result)
