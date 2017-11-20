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

### Render Previews
<p align="center">
    <img src="https://raw.githubusercontent.com/methusael13/phy-ray/master/preview/render01.png" alt="Render01">
</p>
<blockquote><p>
Materials: Plastic, Glass, Metal(Gold) | Samples: 400 | Lights: AreaLights(Disk, Sphere)
</p></blockquote>
</br>

<p align="center">
    <img src="https://raw.githubusercontent.com/methusael13/phy-ray/master/preview/render02.png" alt="Render02">
</p>
<blockquote><p>
Materials: Plastic, Glass, Metal(Copper) | Samples: 400 | Lights: AreaLights(Disk, Sphere)
</p></blockquote>
</br>

<p align="center">
    <img src="https://raw.githubusercontent.com/methusael13/phy-ray/master/preview/render03.png" alt="Render03">
</p>
<blockquote><p>
Materials: Plastic | Samples: 256 | Lights: AreaLights(Disk, Sphere)
</p></blockquote>
</br>

<p align="center">
    <img src="https://raw.githubusercontent.com/methusael13/phy-ray/master/preview/render04.png" alt="Render04">
</p>
<blockquote><p>
Materials: Plastic, Glass, Mirror, Matte, Metal(Silver)  | Samples: 2500 | Lights: AreaLights(Disk)
</p></blockquote>
</br>
