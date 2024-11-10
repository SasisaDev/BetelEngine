#include "Artifact.h"

Artifact Artifact::operator+(const Artifact& otherArtifact)
{
    // Append data bank
    for(auto it = otherArtifact.dataBank.begin(); it != otherArtifact.dataBank.end(); it++) {
        dataBank.insert_or_assign(it->first, it->second);
    }
    return *this;
}