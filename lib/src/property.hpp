#ifndef DILAY_PROPERTY
#define DILAY_PROPERTY

#include <string>

class Property
{
public:
    enum Type
    {
        Bool,
        Float
    };

    Property(Type type, const std::string& name);
    Type type() const;

    void setValue(bool v);
    void setValue(float f);

private:
    const Type m_type;
    std::string m_name;
};

#endif
