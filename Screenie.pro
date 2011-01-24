include(Distribution.pri)

TEMPLATE = subdirs
SUBDIRS = Utils Resources Model Kernel Screenie

Utils.subdir = src/Utils

Resources.subdir = src/Resources
Resources.depends = Utils

Model.subdir = src/Model
Model.depends = Resources Utils

Kernel.subdir = src/Kernel
Kernel.depends = Model Resources Utils

Screenie.subdir = src/Screenie
Screenie.depends = Kernel Model Resources Utils
		  
