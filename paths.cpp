#include "paths.h"

Paths::Paths()
{

}

void Paths::addPath(Path &path) {
    paths.push_back(path);
}

void Paths::removePath(int const& index)   {
    paths.remove(index);
}

void Paths::clear() {
    paths.clear();
}

int Paths::size() const {
    return paths.size();
}
