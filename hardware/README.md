# Hardware

All boards were designed in Autodesk Fusion Electronics (earlier ones in EAGLE). License: [CC BY 4.0](LICENSE).

## Current version

| Folder | Board | Content |
|---|---|---|
| [`robocore-v3.2`](robocore-v3.2) | RoboCore V3.2, Raspberry Pi HAT with ESP32-S3 (4 layers) | schematic, BOM (CSV), [interactive BOM](robocore-v3.2/RoboCore-V3.2_interactive_BOM.html), pinout table, manual (PDF + DOCX), ODB++ fab data, Fusion `.f3d`, STEP, LTspice simulation of the battery divider, renders |
| [`robolight-v4.3`](robolight-v4.3) | RoboLight V4.3 light sensor bar (2 layers) | schematic, BOM (CSV), ODB++ fab data, Fusion `.f3z`, STEP |
| [`mechanical`](mechanical) | 3D-printed parts | light shield V1 (STL) / V2 (3MF), gyro protection (STL), chassis renders |

## Archive

Earlier revisions, kept for reference. They are not maintained.

| Folder | Board | Year | Notes |
|---|---|---|---|
| [`archive/robocore-v3.1`](archive/robocore-v3.1) | RoboCore V3.1 + RoboLight V4.1 | 2024 | first Raspberry Pi HAT version |
| [`archive/robolight-v4.2`](archive/robolight-v4.2) | RoboLight V4.2 | 2024 | regular RGB LEDs |
| [`archive/robocore-v2`](archive/robocore-v2) | RoboCore V2 + RoboCore-Light | 2024 | ESP32, gyro on separate board, changelog PDF |
| [`archive/arche-robocore-v1`](archive/arche-robocore-v1) | Arche-RoboCore V1 | 2023 | first version (EAGLE `.sch` / `.brd`, Gerber, STEP) |
| [`archive/light-sensor-bar-v1`](archive/light-sensor-bar-v1), [`-v2`](archive/light-sensor-bar-v2) | Light sensor bar V1 / V2 | 2023 | EAGLE + Fusion sources, Gerber |
| [`archive/rgb-sensor-v1`](archive/rgb-sensor-v1) | RGB colour sensor | 2023 | EAGLE + Fusion sources, Gerber |

## Fabrication notes

- RoboCore V3.2: 4 layers, 1.6 mm, min. trace width 0.125 mm.
- RoboLight V4.3: 2 layers, 1.2 or 1.6 mm, min. trace width 0.15 mm.
- The fab data of the current boards is ODB++ (e.g. for AISLER). For other manufacturers, export Gerber from the Fusion source files.
