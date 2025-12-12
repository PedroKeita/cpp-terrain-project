/**
 * @file MainWindow.h
 * @brief Main application window for 3D Terrain Generator
 *
 * This header defines the main application window that provides the user interface
 * for drawing terrain profiles and visualizing generated 3D terrains.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QImage>
#include <QSlider>
#include <QLabel>
#include <Eigen/Core>

// Forward declarations to reduce compilation dependencies
class PaintWidget;
class QPushButton;
class GradientField;

/**
 * @class MainWindow
 * @brief Primary application window managing the terrain generation workflow
 *
 * The MainWindow class orchestrates the complete terrain generation process:
 * 1. Provides a drawing canvas for user input
 * 2. Processes the drawing into a heightmap
 * 3. Displays the generated 3D terrain in a separate viewer
 *
 * @note This class follows the Model-View-Controller pattern, acting as the controller
 *       between the UI (view) and the terrain generation algorithms (model).
 */
class MainWindow : public QMainWindow {
    Q_OBJECT  ///< Required for Qt signal/slot mechanism

public:
    /**
     * @brief Construct the main application window
     *
     * Initializes the user interface with two pages:
     * - Page 1: Drawing canvas and controls
     * - Page 2: 3D terrain visualization status
     *
     * @param parent Parent widget (nullptr for top-level window)
     */
    MainWindow(QWidget* parent = nullptr);

public slots:
    /**
     * @brief Slot for generating terrain from user drawing
     *
     * This slot is triggered when the user clicks the "Generate Terrain" button.
     * It performs the complete terrain generation pipeline:
     * 1. Captures the drawing from PaintWidget
     * 2. Processes it through various algorithms (gradient extraction, noise addition, etc.)
     * 3. Applies smoothing and erosion
     * 4. Switches to the visualization page
     */
    void onGenerateTerrain();

    /**
     * @brief Slot for returning to the drawing page
     *
     * Switches from the visualization page back to the drawing page,
     * allowing the user to create a new terrain or modify the current one.
     */
    void onBackToDrawing();

    /**
     * @brief Slot for updating brush size based on slider value
     *
     * @param size New brush size in pixels
     *
     * Updates the PaintWidget's brush size and displays the current size
     * in the brush label. This provides real-time feedback to the user.
     */
    void onBrushSizeChanged(int size);

private:
    /// @name UI Components
    /// @{
    QStackedWidget* stack;          ///< Widget stack managing page transitions
    PaintWidget* paintWidget;       ///< Canvas for drawing terrain profiles
    QWidget* page1;                 ///< Page 1: Drawing interface
    QWidget* page2;                 ///< Page 2: Visualization interface
    QPushButton* generateButton;    ///< Button to trigger terrain generation
    QPushButton* clearButton;       ///< Button to clear the drawing canvas
    QPushButton* backButton;        ///< Button to return to drawing page
    QSlider* brushSlider;           ///< Slider for adjusting brush size
    QLabel* brushLabel;             ///< Label displaying current brush size
    /// @}

    GradientField* sharedGradient = nullptr;
    void applySoftErosion(Eigen::MatrixXd& heightmap, int iterations);

    // Prevent copying to enforce singleton-like behavior
    MainWindow(const MainWindow&) = delete;             ///< Copy constructor disabled
    MainWindow& operator=(const MainWindow&) = delete;  ///< Copy assignment disabled
};

#endif