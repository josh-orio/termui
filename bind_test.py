import sys
sys.path.append("build/bindings")

import termui

t = termui.Strings(["Ketchup", "Mustard", "BBQ"])

m = termui.Menu("Sauce", t, 1)
m.show()

termui.Menu("Sauce", t, 0).show()
