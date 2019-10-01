from xmlrpc.client import ServerProxy
import ssl

class MonitorSettings:
    def __init__(self, server, crendentials):
        self.n4d = ServerProxy('https://{server}:9779'.format(server=server), context=ssl._create_unverified_context(), allow_none=True)
        self.credentials = credentials

    def saveResolution(self, settings, identifier):
        globalSettings = self.n4d.get_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS')
        if globalSettings is None: 
            globalSettings = {'mode':None,'configurations':{}}
            globalSettings['configurations'][identifier] = settings
            self.n4d.objects.add_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['configurations'][identifier] = settings
            self.n4d.set_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS', globalSettings)

    def saveMode(self, mode):
        globalSettings = self.n4d.get_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS')
        if globalSettings is None:
            globalSettings = {'mode': mode,'configurations':{}}
            self.n4d.objects.add_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS', globalSettings, {}, '', '')
        else:
            globalSettings['mode'] = mode
            self.n4d.set_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS', globalSettings)

    def getSettings(self):
        result = self.n4d.get_variable(self.credentials, 'VariablesManager', 'MONITORSETTINGS')
        if result is None:
            result = {'mode':None,'configurations':{}}
        return result

