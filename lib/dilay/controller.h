#ifndef DILAY_CONTROLLER
#define DILAY_CONTROLLER

#include <glm/fwd.hpp>

class ViewProperties;

class Controller {
public:
    virtual void resetProperties() = 0;
    // this->mainWindow->mainWidget ().properties ().reset ();
    virtual void deselectTool() = 0;
    // this->mainWindow->mainWidget ().deselectTool ();
    virtual void update() = 0;
    // this->state.mainWindow ().mainWidget ().glWidget ().update ();
    virtual ViewProperties& viewProperties() = 0;
    // return this->state.mainWindow ().mainWidget ().properties ();
    virtual glm::ivec2 cursorPosition() = 0;
    // return this->state.mainWindow ().mainWidget ().glWidget ().cursorPosition ();

};


#endif
