#pragma once

namespace Ocean_Outlaws::AI {
    /// \n The state of a behaviour tree process.
    /// \SUCCESS when an operation was successful,
    /// \RUNNING if the operation is still ongoing,
    /// \FAILURE if the operation failed.
    enum NodeState {
        RUNNING,
        SUCCESS,
        FAILURE
    };
}
