# PhyRay
A ray tracer based on physically based rendering. Originally based on the pbrt-v3 rendering engine.

#### Status
Project is under active development. No release candidates yet.

#### Dependencies
- OpenEXR

| Platform | Command |
| -------- | ------- |
| Fedora | `sudo dnf install OpenEXR OpenEXR-libs OpenEXR-devel OpenEXR_Viewers` |

### Building
Clone this repository
```
git clone https://github.com/methusael13/phy-ray
```
Create a build directory and compile
```
cd phy-ray
mkdir build
cd build && cmake ..
make
```
Run unit tests
```
make test
```
Render a test scene (defined in `phyray_app/src/main.cpp`)
```
phyray_app/phyrapp <filename>
```
View rendered image
```
exrdisplay <filename>.exr
```
