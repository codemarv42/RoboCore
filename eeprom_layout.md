

# EEPROM Layout
This documents the eeprom slots occupied by this program



| slot  | type      | variable                          |
|-------|-----------|-----------------------------------|
| 0     | bool      | inRescueZone                      |
| 1     | uint8     | victimsRescued                    |
| 2-5   | ?         | _empty_                           |
| | | |
| 6     | int16     | white.left_outer.min              |
| 7     | int16     | white.left_outer.max              |
| 8     | int16     | white.left.min                    |
| 9     | int16     | white.left.max                    |
| 10    | int16     | white.center.min                  |
| 11    | int16     | white.center.max                  |
| 12    | int16     | white.right.min                   |
| 13    | int16     | white.right.max                   |
| 14    | int16     | white.right_outer.min             |
| 15    | int16     | white.right_outer.max             |
| | | |
| 16    | int16     | green.left_outer.min              |
| 17    | int16     | green.left_outer.max              |
| 18    | int16     | green.left.min                    |
| 19    | int16     | green.left.max                    |
| 20    | int16     | green.center.min                  |
| 21    | int16     | green.center.max                  |
| 22    | int16     | green.right.min                   |
| 23    | int16     | green.right.max                   |
| 24    | int16     | green.right_outer.min             |
| 25    | int16     | green.right_outer.max             |
| | | |
| 26    | int16     | red.left_outer.min                |
| 27    | int16     | red.left_outer.max                |
| 28    | int16     | red.left.min                      |
| 29    | int16     | red.left.max                      |
| 30    | int16     | red.center.min                    |
| 31    | int16     | red.center.max                    |
| 32    | int16     | red.right.min                     |
| 33    | int16     | red.right.max                     |
| 34    | int16     | red.right_outer.min               |
| 35    | int16     | red.right_outer.max               |
| | | |
| 36-45 | int16     | _reserved for blue sensor_         |







