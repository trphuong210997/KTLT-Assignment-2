#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
const Position Position::npos = Position(-1, -1);

int ManhattanDistance(Position pos1, Position pos2) {
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
}

Position RobotSMovingRule(int u_distance, int d_distance, int l_distance, int r_distance, Position pos) {
    Position next_pos = pos;
    int count = 0;
    int min = u_distance;
    int min_distance[] = {u_distance, r_distance, d_distance, l_distance};
    for (int i = 0; i < 4; ++i) {
        if (min > min_distance[i]) {
            min = min_distance[i];
            count = i;
        }
    }

    switch (count)
    {
    case 0:
        next_pos.setRow(next_pos.getRow() - 1);
        break;

    case 1:
        next_pos.setCol(next_pos.getCol() + 1);
        break;

    case 2:
        next_pos.setRow(next_pos.getRow() + 1);
        break;

    case 3:
        next_pos.setCol(next_pos.getCol() - 1);
        break;

    default:
        break;
    }
    return next_pos;
}

void CheckHpExp(int & hp, int & exp) {
    if (hp < 0)
        hp = 0;

    if (hp > 500)
        hp = 500;

    if (exp < 0)
        exp = 0;

    if (exp > 900)
        exp = 900;
}

Position CriminalNextPos(int u_distance, int d_distance, int l_distance, int r_distance, Position pos) {
    Position next_pos = pos;

    int count = 0;
    int max = u_distance;
    int max_distance[] = {u_distance, l_distance, d_distance, r_distance};
    for (int i = 0; i < 4; ++i) {
        if (max_distance[i] > max) {
            max = max_distance[i];
            count = i;
        }
    }

    switch (count)
    {
    case 0:
        next_pos.setRow(next_pos.getRow() - 1);
        break;

    case 1:
        next_pos.setCol(next_pos.getCol() - 1);
        break;

    case 2:
        next_pos.setRow(next_pos.getRow() + 1);
        break;

    case 3:
        next_pos.setCol(next_pos.getCol() + 1);
        break;

    default:
        break;
    }
    return next_pos;
}

Position MovingRule(Position pos, string & moving_rule) {
    Position next_pos = pos;

    if (moving_rule[0] == 'U') 
        next_pos.setRow(next_pos.getRow() - 1);
    
    else if (moving_rule[0] == 'D') 
        next_pos.setRow(next_pos.getRow() + 1);
    
    else if (moving_rule[0] == 'L') 
        next_pos.setCol(next_pos.getCol() - 1);
    
    else if (moving_rule[0] == 'R') 
        next_pos.setCol(next_pos.getCol() + 1);

    moving_rule = moving_rule.substr(1) + moving_rule[0];
    return next_pos;
}

Position RobotSW_distance(Sherlock * sherlock, Watson * watson, Position pos) {
    Position next_pos = pos;
    Position Upos = next_pos;
    Upos.setRow(Upos.getRow() - 1);
    int Udistance = ManhattanDistance(Upos, sherlock->getCurrentPosition()) + ManhattanDistance(Upos, watson->getCurrentPosition());

    Position Dpos = next_pos;
    Dpos.setRow(Dpos.getRow() + 1);
    int Ddistance = ManhattanDistance(Dpos, sherlock->getCurrentPosition()) + ManhattanDistance(Dpos, watson->getCurrentPosition());

    Position Lpos = next_pos;
    Lpos.setCol(Lpos.getCol() - 1);
    int Ldistance = ManhattanDistance(Lpos, sherlock->getCurrentPosition()) + ManhattanDistance(Lpos, watson->getCurrentPosition());

    Position Rpos = next_pos;
    Rpos.setCol(Rpos.getCol() + 1);
    int Rdistance = ManhattanDistance(Rpos, sherlock->getCurrentPosition()) + ManhattanDistance(Rpos, watson->getCurrentPosition());

    next_pos = RobotSMovingRule(Udistance, Ddistance, Ldistance, Rdistance, pos);

    return next_pos;
}

bool Map :: isValid(const Position & pos, MovingObject * mv_obj) const {

            if (pos.getRow() < 0 || pos.getRow() > num_rows - 1|| pos.getCol() < 0 || pos.getCol() > num_cols - 1)
                return false;

            if (map[pos.getRow()][pos.getCol()]->getType() == WALL)
                return false;

            if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) {
                FakeWall * fake_wall = dynamic_cast<FakeWall *>(map[pos.getRow()][pos.getCol()]);
                if (fake_wall->getReqExp() > dynamic_cast<Watson*>(mv_obj)->getExp()) 
                    return false;
    
            }
            return true;
}