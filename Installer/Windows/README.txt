Windows Installer XML
=====================


About
-----

WiX is an XML based toolset provided by Microsoft for creating MSI
installation packages. The initial Screenie.wxs build script has
been created with WiX Edit:

  http://wixedit.sourceforge.net/

which itself is based on Windows Installer XML (WiX) toolset:

  http://wix.sourceforge.net/


Installer Creation
------------------

- Install WiX Edit
- Open Screenie.wxs with WiX Edit
- Build (CTRL + B)
- The resulting Screenie.msi can then be installed on any modern
  Windows flavour

Alternatively use the WiX tools "candle.exe" and "light.exe" directly.


License Text
------------

The license text is taken from a given *.rtf Rich Text Format document,
in this case the GPL3.rtf.

DON'T edit this file with MS Word, as this adds more data than necessary
to the document and WiX installer has trouble displaying that document
(the license screen remains blank, unless a button is clicked or so).

Only use WordPad (possibly other RTF editors - OpenOffice might be fine, too).

Links
-----

- WiX Edit: http://wixedit.sourceforge.net/
- Windows Installer XML (WiX): http://wix.sourceforge.net/
- WiX Tutorial: http://www.tramontana.co.hu/wix/
- General info about WiX: https://secure.wikimedia.org/wikipedia/en/wiki/WiX
- Other installers: https://secure.wikimedia.org/wikipedia/en/wiki/List_of_installation_software
- Yet another (free) MSI installer: http://www.advancedinstaller.com/
- "What is the best installer": http://stackoverflow.com/questions/3767/what-is-the-best-choice-for-building-windows-installers

