"""
This script demonstrates how to create a bare-bones, fully functional
chatbot using PyAIML.
"""

import aiml
import os
import sys
import web
from web import form

render = web.template.render('templates/')

urls = (
		'/', 'index',
		'/images/(.*)', 'images'
		)

app = web.application(urls, globals())

myform = form.Form( 
    form.Textbox("said", description="")
    )

class index: 
    def GET(self): 
        form = myform()
        # make sure you create a copy of the form by calling it (line above)
        # Otherwise changes will appear globally
        return render.index(form)

    def POST(self): 
        form = myform()
        if not form.validates(): 
            return render.index(form)
        else:
            # form.d.boe and form['boe'].value are equivalent ways of
            # extracting the validated arguments from the form.
            kern.respond(form['said'].value)
            return render.index(form)


class images:
    def GET(self,name):
        ext = name.split(".")[-1] # Gather extension

        cType = {
            "png":"images/png",
            "jpg":"images/jpeg",
            "gif":"images/gif",
            "ico":"images/x-icon"            }

        if name in os.listdir('images'):  # Security
            web.header("Content-Type", cType[ext]) # Set the Header
            return open('images/%s'%name,"rb").read() # Notice 'rb' for reading images
        else:
            raise web.notfound()

# Create a Kernel object.

kern = aiml.Kernel()

# When loading an AIML set, you have two options: load the original
# AIML files, or load a precompiled "brain" that was created from a
# previous run. If no brain file is available, we force a reload of
# the AIML files.
brainLoaded = False
forceReload = False
while not brainLoaded:
	if forceReload or (len(sys.argv) >= 2 and sys.argv[1] == "reload"):
		# Use the Kernel's bootstrap() method to initialize the Kernel. The
		# optional learnFiles argument is a file (or list of files) to load.
		# The optional commands argument is a command (or list of commands)
		# to run after the files are loaded.
		kern.bootstrap(learnFiles="std-startup.xml", commands="load aiml b")
		brainLoaded = True
		# Now that we've loaded the brain, save it to speed things up for
		# next time.
		kern.saveBrain("standard.brn")
	else:
		# Attempt to load the brain file.  If it fails, fall back on the Reload
		# method.
		try:
			# The optional branFile argument specifies a brain file to load.
			kern.bootstrap(brainFile = "standard.brn")
			brainLoaded = True
		except:
			forceReload = True

# Enter the main input/output loop.
print "\nINTERACTIVE MODE (ctrl-c to exit)"

if __name__=="__main__":
    web.internalerror = web.debugerror
    app.run()
    
# while(True):
# 	print kern.respond(raw_input("> "))
