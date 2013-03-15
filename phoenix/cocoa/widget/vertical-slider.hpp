@interface CocoaVerticalSlider : NSSlider {
@public
  phoenix::VerticalSlider *verticalSlider;
}
-(id) initWith :(phoenix::VerticalSlider&)verticalSlider;
-(IBAction) activate :(id)sender;
@end

namespace phoenix {

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;
  CocoaVerticalSlider *cocoaVerticalSlider;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
};

}
