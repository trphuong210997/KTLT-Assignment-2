/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
// class MovingObject;
// class Position;
// class Configuration;
// class Map;

// class Criminal;
// class RobotS;
// class RobotW;
// class RobotSW;
// class RobotC;

// class ArrayMovingObject;
// class StudyPinkProgram;

// class BaseItem;
// class BaseBag;
// class SherlockBag;
// class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    protected:
        ElementType type;
    public:
        MapElement(ElementType in_type);
        virtual ~MapElement();
        virtual ElementType getType() const;
};

class Path : public MapElement {
    public:
        Path() : MapElement(PATH) {};
};

class Wall : public MapElement {
    public:
        Wall() : MapElement(WALL) {};
};

class FakeWall : public MapElement {
    public:
        int req_exp;

    public:
        FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(in_req_exp) {};

        int getReqExp() const {
            return req_exp;
        }
};

class Map {
    private:
        int num_rows, num_cols;
        MapElement *** map;

    public:
        Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols), map(nullptr){
            map = new MapElement ** [num_rows];
            for (int i = 0; i < num_rows; ++i) {
                map[i] = new MapElement * [num_cols];
                for (int j = 0; j < num_cols; ++j) {
                    map[i][j] = new Path();
                }
            }

            for (int i = 0; i < num_rows; ++i) {
                for (int j = 0; j < num_cols; ++j) {
                    for (int k = 0; k < num_walls; ++k) {
                        if (array_walls[k].isEqual(i, j)) {
                            delete map[i][j];
                            map[i][j] = new Wall();
                            break;
                        }
                    }
                    for (int k = 0; k < num_fake_walls; ++k) {
                        if (array_fake_walls[k].isEqual(i, j)) {
                            delete map[i][j];
                            int req_exp = ((i * 257) + (j * 139) + 89) % 900 + 1;
                            map[i][j] = new FakeWall(req_exp);
                            break;
                        }
                    }
                }
            }
        }

        ~Map() {
            for (int i = 0; i < num_rows; ++i) {
                for (int j = 0; j < num_cols; ++j) {
                    delete map[i][j];
                }
                delete [] map[i];
            }
            delete [] map;
        }

        bool isValid(const Position & pos, MovingObject * mv_obj) const {
            if (pos.getRow() < 0 || pos.getRow() >= num_rows || pos.getCol() < 0 || pos.getCol() >= num_cols) {
                return false;
            }

            if (map[pos.getRow()][pos.getCol()]->getType() == WALL) {
                return false;
            }

            if (map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL) {
                if (dynamic_cast<Watson*>(mv_obj) != nullptr) {       
                        FakeWall * fake_wall = dynamic_cast<FakeWall *>(map[pos.getRow()][pos.getCol()]);
                        if (fake_wall->getReqExp() > dynamic_cast<Watson*>(mv_obj)->getExp()) {
                            return false;
                        }
                    }
            }
            return true;
        }
};

class Position {
    private:
        int r, c;
    public:
        static const Position npos;

        Position(int r=0, int c=0);

        Position(const string & str_pos) {
            int row, col;
            sscanf(str_pos.c_str(), "%d %d", &row, &col);
            this->r = row;
            this->c = col;
        }

        int getRow() const {
            return r;
        }

        int getCol() const {
            return c;
        }

        void setRow(int r) {
            this->r = r;
        }

        void setCol(int c) {
            this->c = c;
        }

        string str() const {
            return "(" + to_string(r) + " " + to_string(c) + ")";
        }

        bool isEqual(int in_r, int in_c) const {
            if (r == in_r && c == in_c) {
                return true;
            }
            return false;
        }
    };

const Position Position::npos = Position(-1, -1);

class MovingObject {
    protected:
        int index;
        Position pos;
        Map * map;
        string name;

    public:
        MovingObject() {}

        MovingObject(int index, const Position pos, Map * map, const string & name="") {
            this->index = index;
            this->pos = pos;
            this->map = map;
            this->name = name;
        }

        virtual ~MovingObject() {}
        virtual Position getNextPosition() = 0;

        Position getCurrentPosition() const {
            return pos;
        }

        virtual void move() = 0;
        virtual string str() const = 0;
};

class Character : public MovingObject {
    public:
        int index;
        Position pos;
        Map * map;
        string name;
    
    public:
        Character(int index, const Position & pos, Map * map, const string & name) : MovingObject(index, pos, map, name) {}
};

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

class Sherlock : public Character {
    protected:
        string moving_rule;
        int hp;
        int exp;

    public:
        Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), hp(init_hp), exp(init_exp) {
            CheckHpExp(hp, exp);
        }

        ~Sherlock() {}

        Position getNextPosition() {
            int r = pos.getRow();
            int c = pos.getCol();

            Position next_pos = MovingRule(pos, moving_rule);

            if (map->isValid(next_pos, this)) {
                return next_pos;
            }
            return Position::npos;
        }

        void move() {
            Position next_pos = getNextPosition();
            if (next_pos.isEqual(-1, -1)) {
                return;
            }
            this->pos = next_pos;
        }

        int getHp() const {
            return hp;
        }

        void setHp(int hp) {
            this->hp = hp;
        }

        int getExp() const {
            return exp;
        }

        void setExp(int exp) {
            this->exp = exp;
        }

        string str() const {
            return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
        }
};

class Watson : public Character {
    private:
        string moving_rule;
        int hp;
        int exp;

    public:
        Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), hp(init_hp), exp(init_exp) {
            CheckHpExp(hp, exp);
        }

        ~Watson() {}

        Position getNextPosition() {
            int r = pos.getRow();
            int c = pos.getCol();

            Position next_pos = MovingRule(pos, moving_rule);

            if (map->isValid(next_pos, this)) {
                return next_pos;
            }
            return Position::npos;
        }

        void move() {
            Position next_pos = getNextPosition();
            if (next_pos.isEqual(-1, -1)) {
                return;
            }
            this->pos = next_pos;
        }

        int getHp() const {
            return hp;
        }

        void setHp(int hp) {
            this->hp = hp;
        }

        int getExp() const {
            return exp;
        }

        void setExp(int exp) {
            this->exp = exp;
        }

        string str() const {
            return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
        }
};

int ManhattanDistance(Position pos1, Position pos2) {
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
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

    delete [] max_distance;
    return next_pos;
}


class Criminal : public Character {
    private:
        Sherlock * sherlock;
        Watson * watson;

    public:
        Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Character(index, init_pos, map, "Criminal") {
            this->sherlock = sherlock;
            this->watson = watson;
        }

        ~Criminal() {}

        Position getNextPosition() {
            Position Upos = this->pos;
            Upos.setRow(Upos.getRow() - 1);
            int Udistance = ManhattanDistance(Upos, sherlock->getCurrentPosition()) + ManhattanDistance(Upos, watson->getCurrentPosition());

            Position Dpos = this->pos;
            Dpos.setRow(Dpos.getRow() + 1);
            int Ddistance = ManhattanDistance(Dpos, sherlock->getCurrentPosition()) + ManhattanDistance(Dpos, watson->getCurrentPosition());

            Position Lpos = this->pos;
            Lpos.setCol(Lpos.getCol() - 1);
            int Ldistance = ManhattanDistance(Lpos, sherlock->getCurrentPosition()) + ManhattanDistance(Lpos, watson->getCurrentPosition());

            Position Rpos = this->pos;
            Rpos.setCol(Rpos.getCol() + 1);
            int Rdistance = ManhattanDistance(Rpos, sherlock->getCurrentPosition()) + ManhattanDistance(Rpos, watson->getCurrentPosition());

            Position next_pos = CriminalNextPos(Udistance, Ddistance, Ldistance, Rdistance, pos);

            if (map->isValid(next_pos, this)) {
                return next_pos;
            }
            return Position::npos;
        }
        

        void move() {
            Position next_pos = getNextPosition();
            if (next_pos.isEqual(-1, -1)) {
                return;
            }
            this->pos = next_pos;
        }

        string str() const {
            return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
        }
};

class ArrayMovingObject {
    private:
        MovingObject * arr_mv_objs;
            int capacity;
            int count;
            MovingObject** arr_mv_objs; 

    public:
        ArrayMovingObject(int capacity) : capacity(capacity), count(0) {
            arr_mv_objs = new MovingObject*[capacity]();
            for (int i = 0; i < capacity; i++) {
                arr_mv_objs[i] = nullptr;
            }
        }

        ~ArrayMovingObject() {
            for (int i = 0; i < count; i++) {
                delete arr_mv_objs[i]; // Delete each object
            }
            delete [] arr_mv_objs; // Delete the array of pointers
        }

        bool isFull() const {
            return count == capacity;
        }

    bool add(MovingObject * mv_obj) {
        if (isFull()) {
            return false;
        }
        arr_mv_objs[count++] = mv_obj;
        return true;
    }

    MovingObject * get(int index) const {
        if (index >= 0 && index < count) {
            return arr_mv_objs[index];
        } else {
            return nullptr;
        }
    }

    int size() const {
        return count;
    }

    string str() const {
        string res = "";
        for (int i = 0; i < count; ++i) {
            res += arr_mv_objs[i].str() + " ";
        }
        return "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";" + res + "]";
    }
};

class Configuration {
    friend class StudyPinkProgram;

    private:
        int map_num_rows, map_num_cols;
        int num_walls;
        int max_num_moving_objects;
        Position * arr_walls;
        int num_fake_walls;
        Position * arr_fake_walls;
        Position sherlock_init_pos;
        string sherlock_moving_rule;
        int sherlock_init_hp;
        int sherlock_init_exp;
        Position watson_init_pos;
        string watson_moving_rule;
        int watson_init_hp;
        int watson_init_exp;
        Position criminal_init_pos;
        int num_steps;

    public:
        Configuration(const string & filepath) {
            ifstream file(filepath);
            if (!file.is_open()) {
                cerr << "Error: cannot open file " << filepath << endl;
                return;
            }

            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string key;
                string rule;
                int value;
                char comma;
                int r, c;

                if (iss >> key >> value) {
                    if (key == "MAP_NUM_ROWS")
                        map_num_rows = value;
                    else if (key == "MAP_NUM_COLS")
                        map_num_cols = value;
                    else if (key == "MAX_NUM_MOVING_OBJECTS")
                        max_num_moving_objects = value;
                    else if (key == "NUM_WALLS")
                        num_walls = value;
                    else if (key == "NUM_FAKE_WALLS")
                        num_fake_walls = value;
                    else if (key == "SHERLOCK_INIT_HP")
                        sherlock_init_hp = value;
                    else if (key == "SHERLOCK_INIT_EXP")
                        sherlock_init_exp = value;
                    else if (key == "WATSON_INIT_HP")
                        watson_init_hp = value;
                    else if (key == "WATSON_INIT_EXP")
                        watson_init_exp = value;
                    else if (key == "NUM_STEPS")
                        num_steps = value;
                    } 
                
                if (iss >> key >> rule) {
                    if (key == "SHERLOCK_MOVING_RULE") 
                        sherlock_moving_rule = rule;
                    else if (key == "WATSON_MOVING_RULE") 
                        watson_moving_rule = rule;
                }
                
                if (iss >> key >> r >> comma >> c) {
                    if (key == "SHERLOCK_INIT_POS") {
                        sherlock_init_pos = Position(r, c);
                    } else if (key == "WATSON_INIT_POS") {
                        watson_init_pos = Position(r, c);
                    } else if (key == "CRIMINAL_INIT_POS") {
                        criminal_init_pos = Position(r, c);
                    }
                }   


            }


            file.close();
        }
        

        ~Configuration() {
            delete [] arr_walls;
            delete [] arr_fake_walls;
        }

        string str() const {
            ostringstream oss;
            oss << "Configuration" << endl;
            oss << "MAP_NUM_ROWS=" << map_num_rows << endl;
            oss << "MAP_NUM_COLS=" << map_num_cols << endl;
            oss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
            oss << "NUM_WALLS=" << num_walls << endl;
            oss << "ARR_WALLS=[" << arr_walls[0].str() << endl;
            oss << "NUM_FAKE_WALLS=" << num_fake_walls << endl;
            oss << "ARR_FAKE_WALLS=[" << arr_fake_walls[0].str() << endl;
            oss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
            oss << "SHERLOCK_INIT_POS=" << sherlock_init_pos.str() << endl;
            oss << "SHERLOCK_INIT_HP=" << sherlock_init_hp << endl;
            oss << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << endl;
            oss << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
            oss << "WATSON_INIT_POS=" << watson_init_pos.str() << endl;
            oss << "WATSON_INIT_HP=" << watson_init_hp << endl;
            oss << "WATSON_INIT_EXP=" << watson_init_exp << endl;
            oss << "CRIMINAL_INIT_POS=" << criminal_init_pos.str() << endl;
            oss << "NUM_STEPS=" << num_steps << endl << "]";
            return oss.str();
        }
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject {
    public:
        RobotType type;
        BaseItem * item;
    
    public:
        Robot(RobotType in_type) : type(in_type) {}


};

class BaseItem {
    protected:
        ItemType type;
    public:
        BaseItem(ItemType in_type) : type(in_type) {}
        virtual ~BaseItem() {}
        virtual ItemType getType() const {
            return type;
        }

        virtual bool canUse(Character * obj, Robot * robot) const = 0;
        virtual void use(Character* obj , Robot * robot) = 0;
};

class MagicBook : public BaseItem {
    public:
        MagicBook() : BaseItem(MAGIC_BOOK) {}

        bool canUse(Character * obj, Robot * robot) const {
            if (dynamic_cast<Watson*>(obj) != nullptr) {
                if (dynamic_cast<Watson*>(obj)->getExp() <= 350) {
                    return true;
                }
            }

            else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                if (dynamic_cast<Sherlock*>(obj)->getExp() <= 350) {
                    return true;
                }
            }
            return false;
        }

        void use(Character * obj, Robot * robot) {
            if (canUse(obj, robot)) {
                if (dynamic_cast<Watson*>(obj) != nullptr) {
                    dynamic_cast<Watson*>(obj)->setExp(dynamic_cast<Watson*>(obj)->getExp() * 1.25);
                }
                else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                    dynamic_cast<Sherlock*>(obj)->setExp(dynamic_cast<Sherlock*>(obj)->getExp() * 1.25);
                }
            }
        }
};

class EnergyDrink : public BaseItem {
    public:
        EnergyDrink() : BaseItem(ENERGY_DRINK) {}

        bool canUse(Character * obj, Robot * robot) const {
            if (dynamic_cast<Watson*>(obj) != nullptr) {
                if (dynamic_cast<Watson*>(obj)->getHp() <= 100) {
                    return true;
                }
            }

            else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                if (dynamic_cast<Sherlock*>(obj)->getHp() <= 100) {
                    return true;
                }
            }
            return false;
        }

        void use(Character * obj, Robot * robot) {
            if (canUse(obj, robot)) {
                if (dynamic_cast<Watson*>(obj) != nullptr) {
                    dynamic_cast<Watson*>(obj)->setHp(dynamic_cast<Watson*>(obj)->getHp() * 1.2);
                }
                else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                    dynamic_cast<Sherlock*>(obj)->setHp(dynamic_cast<Sherlock*>(obj)->getHp() * 1.2);
                }
            }
        }
};

class FirstAid : public BaseItem {
    public:
        FirstAid() : BaseItem(FIRST_AID) {}

        bool canUse(Character * obj, Robot * robot) const {
            if (dynamic_cast<Watson*>(obj) != nullptr) {
                if (dynamic_cast<Watson*>(obj)->getHp() <= 100 || dynamic_cast<Watson*>(obj)->getExp() <= 350) {
                    return true;
                }
            }

            else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                if (dynamic_cast<Sherlock*>(obj)->getHp() <= 100 || dynamic_cast<Sherlock*>(obj)->getExp() <= 350) {
                    return true;
                }
            }
            return false;
        }

        void use(Character * obj, Robot * robot) {
            if (canUse(obj, robot)) {
                if (dynamic_cast<Watson*>(obj) != nullptr) {
                    dynamic_cast<Watson*>(obj)->setHp(dynamic_cast<Watson*>(obj)->getHp() * 1.5);
                }
                else if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                    dynamic_cast<Sherlock*>(obj)->setHp(dynamic_cast<Sherlock*>(obj)->getHp() * 1.5);
                }
            }
        }
};

class ExemptionCard : public BaseItem {
    public:
        ExemptionCard() : BaseItem(EXCEMPTION_CARD) {}

        bool canUse(Character * obj, Robot * robot) const {
            if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                if (dynamic_cast<Sherlock*>(obj)->getExp() % 2 != 0) {
                    return true;
                }
            }
            return false;
        }

        void use(Character * obj, Robot * robot) {
            if (dynamic_cast<Sherlock*>(obj) != nullptr) {
                dynamic_cast<Sherlock*>(obj)->setExp(900);
            }
        }
        
};

class PassingCard : public BaseItem {
    public:
        string challenge;
    
    public:
        PassingCard(string challenge_type) : BaseItem(PASSING_CARD), challenge(challenge_type) {}

        bool canUse(Character * obj, Robot * robot) const {
            if (dynamic_cast<Watson*>(obj) != nullptr) {
                if (dynamic_cast<Watson*>(obj)->getExp() % 2 == 0) {
                    return true;
                }
            }
            return false;
        }

        void use(Character * obj, Robot * robot) {
            if (dynamic_cast<Watson*>(obj) != nullptr) {
                if (challenge != robot->getType()) {
                    dynamic_cast<Watson*>(obj)->setExp(dynamic_cast<Watson*>(obj)->getExp() - 50);
                }
            }
        }
};

class StudyPinkProgram {
    private:
        // Sample attributes
        Configuration * config;
        
        Sherlock * sherlock;
        Watson * watson;
        Criminal * criminal;
        
        Map * map;
        ArrayMovingObject * arr_mv_objs;


    public:
        StudyPinkProgram(const string & config_file_path);

        bool isStop() const;

        void printResult() const {
            if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
                cout << "Sherlock caught the criminal" << endl;
            }
            else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
                cout << "Watson caught the criminal" << endl;
            }
            else {
                cout << "The criminal escaped" << endl;
            }
        }

        void printStep(int si) const {
            cout << "Step: " << setw(4) << setfill('0') << si
                << "--"
                << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
        }

        void run(bool verbose) {
            // Note: This is a sample code. You can change the implementation as you like.
            // TODO
            for (int istep = 0; istep < config->num_steps; ++istep) {
                for (int i = 0; i < arr_mv_objs->size(); ++i) {
                    arr_mv_objs->get(i)->move();
                    if (isStop()) {
                        printStep(istep);
                        break;
                    }
                    if (verbose) {
                        printStep(istep);
                    }
                }
            }
            printResult();
        }

        ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
