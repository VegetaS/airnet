#ifndef COMMON_COPYABLE_H_
#define COMMON_COPYABLE_H_

namespace common {

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
};

};

#endif  // COMMON_COPYABLE_H_
