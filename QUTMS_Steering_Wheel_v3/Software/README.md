# Steering Wheel Software

The steering wheel could be considered a dumb terminal that shows the status of the car. It will be the only way to view the systems in the car aside from the C# program.

It will allow you to shift though the multiple modes that the Chassis Controller ([QUTMS_ChassisController](https://github.com/Technosasquach/QUTMS_ChassisController "QUTMS_ChassisController")) is capable off. It would then update the display and dash to what is best suited for the mode. Additonaly it could be programmed with different interfaces that show various elements of the car to the driver while racing.

If you are working on this code, then its recommended that you talk to someone that would be driving, so you can tailor the software to their needs.

Below is a rough outline of its operations procedures.

![Steering Wheel](/Images/operation_procedures.png)

## Dashboard Design Analysis

Based on a team survey, these are the features of highest priority that need to be implemented into the steering wheel software.

| Highest Priority Features | N&deg;| Additional Info |
| :------------------------ |:-----:| :-------------- |
| Speed                     | 14    |                 |
| Battery amount            | 11    | kWh/km?         |
| Heat                      | 9     | Endurance, accel, skidpad, 2WD, 4WD |
| Emergency info overrride  | 6     |                 |
| Race mode                 | 5     |                 |
| Lap timer / #             | 5     |                 |
| Torque output             | 3     |                 |
| Energy efficiency         | 3     |                 |
| Accumulator energy        | 2     |                 |
| Accel, brake levels       | 2     |                 |

These are additional, lower priority features.

| Extra Features              | N&deg; | Additional Info |
| :-------------------------- |:------:| :-------------- |
| Party Parrot loading screen | 6      |                 |
| Start / stop kill switch    | 1      |                 |
| Lateral G's                 | 1      | Car lean        |
| VTEC engaged                | 1      | Tacho           |
| Tyre pressure               | 1      |                 |
| Regen efficiency            | 1      |                 |
| Radio button                | 1      |                 |
| Brake bias adjust (auto)    | 1      |                 |
| Throttle application        | 1      |                 |
| Traction/power per wheel    | 1      |                 |
| Button input (+interactivity) and parameter control | 1 |

**Comments:**

- Big letters and numbers
- Colours used (appropriately)
- UI intuitive
- Wheel rotates around the dashboard, so no upside-down screens
- Clean and concise display - nothing unnecessary
- Diagnostic screen display relevant to an extreme condition (excessive G's &rarr; G-force reading warning)
- Set of comprehensive dashboard/diagnostic screens
- Ergonomic, light-weight, robust
- Input config/check config without config software
- Light predictive elements (fuel, temp)