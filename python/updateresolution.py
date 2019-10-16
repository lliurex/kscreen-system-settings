from xmlrpc.client import ServerProxy
import ssl
import os
import sys
import pyinotify
import json
import syslog
import codecs

home = os.environ['HOME']
masterkeypath = '{home}/.config/kscreensystem'.format(home=home)

if not os.path.exists(masterkeypath):
    sys.exit(0)

n4d = ServerProxy('https://localhost:9779', context=ssl._create_unverified_context(), allow_none=True)

with codecs.open(masterkeypath,'r',encoding='utf-8') as fd:
    masterkey = fd.readline().strip()

class EventHandler(pyinotify.ProcessEvent):
    def my_init(self, n4d, masterkey):
        self.n4d = n4d
        self.masterkey = masterkeypath

    def process_IN_CLOSE_WRITE(self, event):
        self.saveConfig(event.pathname)

    def saveConfig(self,identifier):
        config = {}
        try :
            with codecs.open(identifier,'r',encoding='utf-8') as fd:
                config = json.load(fd)
            print(os.path.basename(identifier),config)
            #result = self.n4d.updateResolution('','MonitorSettings',os.path.basename(identifier), config, self.masterkey)
            if result['status']:
                self.masterkey = result['msg']
                with codecs.open(masterkeypath,'w',encoding='utf-8') as fd:
                    fd.write(self.masterkey)

        except FileNotFoundError:
            pass
        except Exception as e:
            syslog.syslog(str(e))

wm = pyinotify.WatchManager()
handler = EventHandler(n4d=n4d, masterkeypath=masterkey)
notifier = pyinotify.Notifier(wm, default_proc_fun=handler)
wm.add_watch('{home}/.local/share/kscreen'.format(home=home), pyinotify.IN_CLOSE_WRITE)
notifier.loop()