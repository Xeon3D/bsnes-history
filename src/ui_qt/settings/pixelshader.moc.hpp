class PixelShaderWindow : public QWidget {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QLabel *title;
  QGridLayout *gridLayout;
  QLabel *fragmentLabel;
  QLineEdit *fragmentPath;
  QPushButton *fragmentSelect;
  QPushButton *fragmentDefault;
  QLabel *vertexLabel;
  QLineEdit *vertexPath;
  QPushButton *vertexSelect;
  QPushButton *vertexDefault;

  void synchronize();
  PixelShaderWindow();

public slots:
  void selectFragmentShader();
  void selectVertexShader();
  void defaultFragmentShader();
  void defaultVertexShader();

  void assignFragmentShader(string);
  void assignVertexShader(string);
} *pixelShaderWindow;
