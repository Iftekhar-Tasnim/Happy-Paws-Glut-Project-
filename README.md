# Happy Paws 🐱

A delightful computer graphics project featuring an interactive cat feeding simulation built with OpenGL and GLUT.

## 📖 About

Happy Paws is a computer graphics group project that simulates a virtual pet experience. The main feature is an interactive feeding screen where users can click on different food items to feed a cute animated cat. The project demonstrates various computer graphics concepts including 2D rendering, animations, user interaction, and sound integration.

## ✨ Features

### 🎮 Interactive Feeding System
- **Click-to-Feed**: Click on food items (fish, meat drumstick, watermelon) to feed the cat
- **Smooth Animations**: Food items smoothly animate from plates to the cat's mouth
- **Eating Animation**: Cat performs realistic eating animations with mouth movements
- **Sound Effects**: Meowing sounds play when the cat eats

### 🎨 Visual Elements
- **Animated Cat**: Detailed cat character with realistic features
  - Blinking eyes during eating
  - Animated mouth movements
  - Whiskers, ears, and collar details
- **Dynamic Environment**: 
  - Animated clouds moving across the sky
  - Swaying tree branches
  - Gradient backgrounds
- **Interactive Clock**: Functional clock display with moving hands
- **Window View**: Scenic outdoor view visible through a window

### 🎵 Audio Integration
- **Background Music**: Ambient background music during gameplay
- **Sound Effects**: Cat meowing sounds during feeding

### 🖥️ Multiple Screens
- **Intro Screen**: Project information and team details
- **Feeding Screen**: Main interactive gameplay
- **Additional Screens**: Dance, Sleep, Shower, and Fun screens (placeholder)

## 🛠️ Technology Stack

- **OpenGL**: 2D graphics rendering
- **GLUT**: Window management and user input
- **C++**: Core programming language
- **Windows Multimedia API**: Audio playback
- **Code::Blocks**: Development environment

## 📋 Prerequisites

Before running this project, ensure you have:

- **Windows OS** (project uses Windows-specific audio APIs)
- **OpenGL** libraries
- **GLUT** (OpenGL Utility Toolkit)
- **C++ compiler** (GCC, MinGW, or Visual Studio)
- **Code::Blocks IDE** (recommended for easy setup)

## 🚀 Installation

### Method 1: Using Code::Blocks (Recommended)

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/happy-paws.git
   cd happy-paws
   ```

2. **Open in Code::Blocks**
   - Open `HappyPaws.cbp` file in Code::Blocks IDE
   - The project should automatically configure with all necessary libraries

3. **Build and Run**
   - Press `F9` or click "Build and Run" button
   - The application should compile and launch

### Method 2: Manual Compilation

1. **Install Dependencies**
   - Install OpenGL and GLUT libraries
   - Ensure audio libraries are available

2. **Compile the Project**
   ```bash
   g++ -o HappyPaws main.cpp -lopengl32 -lfreeglut -lglu32 -lwinmm
   ```

3. **Run the Application**
   ```bash
   ./HappyPaws
   ```

## 🎮 How to Play

### Controls
- **'1'**: Return to Intro Screen
- **'2'**: Go to Feeding Screen (main gameplay)
- **'3'**: Go to Dance Screen
- **'4'**: Go to Sleep Screen
- **'5'**: Go to Shower Screen
- **'6'**: Go to Fun Screen
- **Mouse Click**: Click on food items to feed the cat

### Gameplay
1. Press **'2'** to enter the feeding screen
2. Click on any food item (fish, meat, or watermelon) on the plates
3. Watch the food animate to the cat's mouth
4. Enjoy the cat's eating animation and meowing sounds
5. Food will reappear after the cat finishes eating

## 👥 Team Members

| Name |
|------|
| Iftekhar Tasnim Md. Asif |


## 📁 Project Structure

```
HappyPaws/
├── main.cpp              # Main source code
├── HappyPaws.cbp         # Code::Blocks project file
├── audio/                # Audio files directory
│   ├── bg.wav           # Background music
│   └── meow.wav         # Cat meowing sound
├── bin/                  # Compiled executable
│   └── Debug/
│       └── HappyPaws.exe
├── obj/                  # Object files
└── doxygen/              # Documentation
    └── doxyfile
```

## 🎯 Key Features Implemented

### Computer Graphics Concepts
- **2D Primitive Drawing**: Circles, rectangles, lines, and polygons
- **Transformations**: Translation, rotation, and scaling
- **Animation**: Frame-based animations with timers
- **User Interaction**: Mouse and keyboard input handling
- **Color Management**: RGB color system with gradients
- **Viewport Management**: Scissor testing for window effects

### Interactive Elements
- **Collision Detection**: Mouse click detection on food items
- **State Management**: Screen transitions and game states
- **Timer Functions**: Smooth animations and timing control

## 🔧 Customization

### Adding New Food Types
To add new food items, modify the `drawFoodItem()` function in `main.cpp`:

```cpp
else if (i == 3) {
    // New food type
    glColor3f(1.0f, 0.5f, 0.0f); // Orange color
    // Add drawing code here
}
```

### Modifying Cat Appearance
Edit the `drawCat()` function to change the cat's appearance, colors, or animations.

### Adding New Screens
1. Add new screen type to the `ScreenType` enum
2. Create a new drawing function
3. Add case in the `display()` function
4. Add keyboard shortcut in `keyboard()` function

## 🐛 Troubleshooting

### Common Issues

**"Cannot find GL/glut.h"**
- Ensure GLUT is properly installed
- Check include paths in your IDE

**Audio not playing**
- Verify audio files are in the correct directory
- Check Windows audio settings

**Window not displaying properly**
- Ensure OpenGL drivers are up to date
- Check graphics card compatibility

### Build Errors
- Make sure all required libraries are linked
- Verify compiler settings match your system architecture

## 📝 License

This project is created for educational purposes as part of a Computer Graphics course.

## 🤝 Contributing

This is an academic project, but suggestions and improvements are welcome through issues and discussions.

## 📞 Contact

For questions or support, please contact the project team or create an issue in the repository.

---

**Happy Paws** - Bringing joy through interactive computer graphics! 🐾
