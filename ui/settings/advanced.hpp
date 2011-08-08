struct AdvancedSettings {
  HorizontalLayout panelLayout;
  Widget panel;
  VerticalLayout layout;
  Label title;

  Label driverSelectionLabel;
  HorizontalLayout driverLayout;

  Label videoDriverLabel;
  ComboBox videoDriverBox;
  Label audioDriverLabel;
  ComboBox audioDriverBox;
  Label inputDriverLabel;
  ComboBox inputDriverBox;

  Label focusPolicyLabel;
  HorizontalLayout focusPolicyLayout;
  RadioBox focusPolicyPause;
  RadioBox focusPolicyIgnore;
  RadioBox focusPolicyAllow;

  Label compositorPolicyLabel;
  HorizontalLayout compositorPolicyLayout;
  RadioBox compositorPolicyNever;
  RadioBox compositorPolicyFullScreen;
  RadioBox compositorPolicyAlways;

  Widget spacer;

  void create();
};

extern AdvancedSettings advancedSettings;
