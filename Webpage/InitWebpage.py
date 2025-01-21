import re
import os



ScriptDirectory = os.path.dirname(os.path.abspath(__file__))

with open(f"{ScriptDirectory}/Webpage.html", 'r', encoding='utf-8') as Source:
    with open(f"{ScriptDirectory}/../Firmware/include/Webpage.h", 'w') as Destination:

        sourceText = "".join(Source.readlines())

        Destination.write("""#include <Arduino.h>\n\nconst char Webpage[] = """)
        
        Destination.write(f'R"(\n{sourceText}\n)";')



# Data Keys for JSON
# Slider    S
# Toggle    T
# Joystick  J
# Knob      K