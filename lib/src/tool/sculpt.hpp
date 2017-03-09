/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_TOOL_SCULPT
#define DILAY_TOOL_SCULPT

#include "tool.hpp"

class QString;
class SculptBrush;
class ToolUtilMovement;
class ViewCursor;

class ToolSculpt : public Tool {
  public:
    DECLARE_BIG2 (ToolSculpt, State&, const char*)

	float radius() const;
	void  radius(float r);

	bool absoluteRadius() const;
	void absoluteRadius(bool b);

	bool subdivide() const;
	void subdivide(bool b);

	void syncMirror();

  protected:
	SculptBrush& brush                    ();
    ViewCursor&  cursor                   ();
    void         addDefaultToolTip        (ViewToolTip&, bool) const;
    void         addSecSliderWheelToolTip (ViewToolTip&, const QString&) const;
    void         sculpt                   ();
    bool         carvelikeStroke          ( const ViewPointingEvent&, bool
                                          , const std::function <void ()>* = nullptr );
    bool         initializeDraglikeStroke (const ViewPointingEvent&, ToolUtilMovement&);
    bool         draglikeStroke           (const ViewPointingEvent&, ToolUtilMovement&);

  private:
    IMPLEMENTATION

    ToolResponse runInitialize    ();
    void         runRender        () const;
    ToolResponse runPointingEvent (const ViewPointingEvent&);
    ToolResponse runCursorUpdate  (const glm::ivec2&);
    void         runFromConfig    ();

    virtual const char* key                    () const = 0;
    virtual void        runSetupBrush          (SculptBrush&) = 0;
    virtual void        runSetupCursor         (ViewCursor&) = 0;
    virtual void        runSetupToolTip        (ViewToolTip&) = 0;
    virtual bool        runSculptPointingEvent (const ViewPointingEvent&) = 0;
};

#define DECLARE_TOOL_SCULPT(name,theKey,accessors)                                 \
  class name : public ToolSculpt { public:                                         \
    DECLARE_BIG2 (name, State&)                                                    \
    private:                                                                       \
      IMPLEMENTATION                                                               \
      const char* key                    () const { return theKey ; }              \
      void        runSetupBrush          (SculptBrush&);                           \
      void        runSetupCursor         (ViewCursor&);                            \
      void        runSetupProperties     (ViewTwoColumnGrid&);                     \
      void        runSetupToolTip        (ViewToolTip&);                           \
      bool        runSculptPointingEvent (const ViewPointingEvent&);               \
    public:                                                                        \
      accessors                                                                    \
  };

#define DECLARE_TOOL_SCULPT_PARAM(type, name)                                       \
    type name ();                                                                   \
    void name (type);

#define DELEGATE_TOOL_SCULPT_PARAM(tool, type, name, brushType)                     \
    type tool :: name () { return brush ().parameters < brushType > (). name (); }  \
    void tool :: name (type t) {                                                    \
        brush ().parameters < brushType > (). name ( t );                           \
        cache ().set ( #name , t);                                                  \
    }

#define DELEGATE_TOOL_SCULPT(name)                                                   \
  DELEGATE_BIG2_BASE (name, (State& s), (this), ToolSculpt, (s,this->key ()))        \
  DELEGATE1 (void, name, runSetupBrush, SculptBrush&);                               \
  DELEGATE1 (void, name, runSetupCursor, ViewCursor&);                               \
  DELEGATE1 (void, name, runSetupToolTip, ViewToolTip&);                             \
  DELEGATE1 (bool, name, runSculptPointingEvent, const ViewPointingEvent&)

#endif
