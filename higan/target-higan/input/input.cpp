#include "../higan.hpp"

namespace Instances { Instance<InputManager> inputManager; }
InputManager& inputManager = Instances::inputManager();

auto InputManager::create() -> void {
  root = interface->root();

  input.create("SDL");
  input.setContext(systemManager.handle());
  input.onChange({&InputManager::eventInput, this});
}

auto InputManager::reset() -> void {
  devices = {};
  input.reset();
}

auto InputManager::poll() -> void {
  //polling actual hardware is very time-consuming: skip call if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < pollFrequency) return;
  lastPoll = thisPoll;

  //poll hardware, detect when the available devices have changed:
  //existing in-use devices may have been disconnected; or mapped but disconnected devices may now be available.
  //as such, when the returned devices tree changes, rebind all inputs
  auto devices = input.poll();
  bool changed = devices.size() != this->devices.size();
  if(!changed) {
    for(uint n : range(devices.size())) {
      if(changed = devices[n] != this->devices[n]) break;
    }
  }
  if(changed) {
    this->devices = devices;
    bind();
  }
}

auto InputManager::bind() -> void {
  for(auto& input : root->find<higan::Node::Input>()) {
    //not strictly necessary; but release any shared_pointer instances to free up the allocated memory
    input->setProperty<shared_pointer<HID::Device>>("device");

    auto _pathID = input->property("pathID"); if(!_pathID) continue;
    auto _vendorID = input->property("vendorID"); if(!_vendorID) continue;
    auto _productID = input->property("productID"); if(!_productID) continue;

    auto pathID = _pathID.natural();
    auto vendorID = _vendorID.natural();
    auto productID = _productID.natural();

    for(auto& device : devices) {
      if(pathID != device->pathID()) continue;
      if(vendorID != device->vendorID()) continue;
      if(productID != device->productID()) continue;

      input->setProperty<shared_pointer<HID::Device>>("device", device);
      break;
    }
  }
}

auto InputManager::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  systemManager.eventInput(device, group, input, oldValue, newValue);
}
