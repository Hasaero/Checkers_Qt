#include "Move.h"
extern bool isBlueTurn;
extern string moveFromButtonName;
extern string moveToButtonName;
extern QLabel* movingLabelPtr;
extern map<string, QLabel*> map_s_Q;
extern int hitPieceCount_b;
extern int hitPieceCount_y;

// The implementation of the 'pickPiece' method from Class Move.
void Move::pickPiece(std::string pickedButtonName, QLabel*& pickedLabelPtr) {
    if (pickedLabelPtr != nullptr) { // When the label exists...
        string pickedLabelcolor = pickedLabelPtr->objectName().toStdString().substr(0,1);

        if (isBlueTurn && pickedLabelcolor== "y") { // When it is Blue's turn of picked Label is yellow, throw an exception.
            throw invalid_argument("This is Blue's turn");
        } else if (!isBlueTurn && pickedLabelcolor== "b") { // When it is Yellow's turn of picked Label is blue, throw an exception.
            throw invalid_argument("This is Yellow's turn");
        } else {
            moveFromButtonName = pickedButtonName; // Store the name as starting point of the move.
            movingLabelPtr = pickedLabelPtr; // Store the pointer of moved piece.
        }
    } else { // If the label does not exist, throw and exception. (There is no label)
        throw invalid_argument("You clicked empty space");
    }
}

// The implementation of the 'movePiece' method from Class Move.
void Move::movePiece(QLabel* movingLabelPtr, string moveFromButtonName, string pickedButtonName){
    // Extract the coordinate of the starting point and potential reaching point from button names.
    // Button: B_x_y
    int fromX = stoi(moveFromButtonName.substr(2,1));
    int fromY = stoi(moveFromButtonName.substr(4,1));

    int toX = stoi(pickedButtonName.substr(2,1));
    int toY = stoi(pickedButtonName.substr(4,1));

    // If the move is not diagonal move, throw an exception.
    if (abs(toX-fromX) != abs(toY-fromY)) {throw invalid_argument("Only diagonal move is allowed");}

    // If the piece is not King...
    if (movingLabelPtr->property("king") == false){
        // If the moveing is not, forward move, throw an exception.
        if (isBlueTurn? toY>=fromY: toY<=fromY){
            throw invalid_argument("Only forward moving is allowed for non-king pieces");
        }
        // If user try to jump more than 3 diagonal squares at once, throw an exception.
        if (abs(toX-fromX) >= 3){
            throw invalid_argument("This is invalid movement");
        }
        // If user try to move 2 diagonal squares...(when capturing an enemy)
        if (abs(toX-fromX) ==2 && abs(toY-fromY)==2){
            string midX = to_string(fromX+(int)(toX-fromX)/2);
            string midY = to_string(fromY+(int)(toY-fromY)/2);
            string midButtonName = "B_";
            midButtonName.append(midX);
            midButtonName.append("_");
            midButtonName.append(midY);
            // Check the label located at the very button in between.
            QLabel* midLabelPtr = map_s_Q.at(midButtonName);

            // If nothing is located on the button, throw an exception.
            if (midLabelPtr == nullptr) {
                throw invalid_argument("No piece in between");
            } //early return

            // If the label located on the button in between has different color, remove it and move the picked piece.
            string color = midLabelPtr->objectName().toStdString().substr(0,1);
            moveToButtonName = pickedButtonName;
            // When blue catchs the yellow one
            if (isBlueTurn && color=="y"){
                movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64);
                QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName); // move the piece
                map_s_Q[moveFromButtonName] = nullptr;
                map_s_Q[moveToButtonName] = tempLabelPtr; // a label newly corresponds to the board button

                map_s_Q.at(midButtonName)->setVisible(false); // remove the captured one
                map_s_Q[midButtonName] = nullptr; // an existing thing is erased
                hitPieceCount_b++; // Increase the hit count.
                return;
            // when yellow catchs the blue one
            } else if (!isBlueTurn && color=="b") {
                movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64); // because, B_1_1: (35,30)
                QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName);
                map_s_Q[moveFromButtonName] = nullptr;
                map_s_Q[moveToButtonName] = tempLabelPtr;

                map_s_Q.at(midButtonName)->setVisible(false);
                map_s_Q[midButtonName] = nullptr;
                hitPieceCount_y++; // Increase the hit count
                return;
            //  If not, throw an exception. (When trying to eat up their own piece.)
            } else{throw invalid_argument("This is invalid movement");}
        }

        // If not filered by the conditions above, it is simply a move(not catching any piece).
        // (Move one square diagonally)
        moveToButtonName = pickedButtonName;
        movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64);
        QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName);
        map_s_Q[moveFromButtonName] = nullptr;
        map_s_Q[moveToButtonName] = tempLabelPtr;
        return;
    }
    else { // When the piece is King, it can go backwards
        if (abs(toX-fromX) ==2 && abs(toY-fromY)==2){ // If user try to move 2 diagonal squares...
            string midX = to_string(fromX+(int)(toX-fromX)/2);
            string midY = to_string(fromY+(int)(toY-fromY)/2);
            string midButtonName = "B_";
            midButtonName.append(midX);
            midButtonName.append("_");
            midButtonName.append(midY);
            QLabel* midLabelPtr = map_s_Q.at(midButtonName);

            // If the label located on the button in between has different color, remove it and move the picked picked.
            if (midLabelPtr != nullptr) {
                string color = midLabelPtr->objectName().toStdString().substr(0,1);
                moveToButtonName = pickedButtonName;
                if (isBlueTurn && color=="y"){
                    movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64);
                    QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName);
                    map_s_Q[moveFromButtonName] = nullptr;
                    map_s_Q[moveToButtonName] = tempLabelPtr;

                    map_s_Q.at(midButtonName)->setVisible(false);
                    map_s_Q[midButtonName] = nullptr;
                    hitPieceCount_b++; // Increase the count.
                    return;
                } else if (!isBlueTurn && color=="b") {
                    movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64);
                    QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName);
                    map_s_Q[moveFromButtonName] = nullptr;
                    map_s_Q[moveToButtonName] = tempLabelPtr;

                    map_s_Q.at(midButtonName)->setVisible(false);
                    map_s_Q[midButtonName] = nullptr;
                    hitPieceCount_y++; // Increase the count.
                    return;
                }
            }
        }
        // If not filered by the conditions above, it is simply a move(not catching any piece).
        moveToButtonName = pickedButtonName;
        movingLabelPtr->move(35+(toX-1)*64, 30+(toY-1)*64);
        QLabel* tempLabelPtr = map_s_Q.at(moveFromButtonName);
        map_s_Q[moveFromButtonName] = nullptr;
        map_s_Q[moveToButtonName] = tempLabelPtr;
        return;
    }
}


