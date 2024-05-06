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
class MovingObject;
class Position;
class Configuration;
class Map;
class Sherlock;
class Watson;
class Criminal;
class Robot; 
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
    protected:
        ElementType type;
    public:
        MapElement(ElementType in_type) : type(in_type) {};
        virtual ~MapElement() {}
        virtual ElementType getType() const {
            return type;
        }
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
    private:
        int req_exp;

    public:
        FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(in_req_exp) {};

        int getReqExp() const {
            return req_exp;
        }
};

class Position {
    friend class TestStudyInPink;
    private:
        int r, c;
    public:
        static const Position npos;

        Position(int r=0, int c=0) {
            this->r = r;
            this->c = c;
        }

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

        void operator&=(const Position & other) {
            this->r = other.r;
            this->c = other.c;
        }

        string str() const {
            ostringstream output;
            output << "(" << r << "," << c << ")";
            return output.str();
        }

        bool isEqual(int in_r, int in_c) const {
            if (r == in_r && c == in_c) {
                return true;
            }
            return false;
        }
};

class MovingObject {
    friend class TestStudyInPink;
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
            return this->pos;
        }

        virtual string getName() const {
            return name;
        }

        virtual void move() = 0;
        virtual string str() const = 0;
};

class Map {
    friend class TestStudyInPink;
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
                        }
                    }
                }
            }
            
            for (int i = 0; i < num_rows; ++i) {
                for (int j = 0; j < num_cols; ++j) {
                    for (int k = 0; k < num_fake_walls; ++k) {
                        if (array_fake_walls[k].isEqual(i, j)) {
                            delete map[i][j];
                            int req_exp = ((i * 257) + (j * 139) + 89) % 900 + 1;
                            map[i][j] = new FakeWall(req_exp);
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

        int getFakeWallExpRequirement(int row, int col) const {
            if (map[row][col]->getType() == FAKE_WALL) {
                return static_cast<FakeWall*>(map[row][col])->getReqExp();
            } else {
                return -1;
            }
        }

        bool isValid(const Position & pos, MovingObject * mv_obj) const;
};

class Character : public MovingObject {
    public:
        int index;
        Position pos;
        Map * map;
        string name;
    
    public:
        Character() {}
        Character(int index, const Position & pos, Map * map, const string & name) : MovingObject(index, pos, map, name) {}
};

void CheckHpExp(int & hp, int & exp) ;

Position MovingRule(Position pos, string & moving_rule) ;

class Sherlock : public Character {
    private:
        int hp;
        int exp;

    public:
        string moving_rule;
        string str_moving_rule;

    public:
        Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : moving_rule(moving_rule), hp(init_hp), exp(init_exp) {
            CheckHpExp(hp, exp);
            this->name = "Sherlock";
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->str_moving_rule = moving_rule;
        }

        Position getNextPosition() {
            int r = pos.getRow();
            int c = pos.getCol();

            Position next_pos = MovingRule(pos, moving_rule);

            if (map->isValid(next_pos, this)) {
                return next_pos;
            }
            return Position::npos;
        }

        Position getCurrentPosition() const {
            return this->pos;
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
            ostringstream output;
            output << "Sherlock[index=" << index << ";pos=" << pos.str() << ";moving_rule=" << str_moving_rule << "]";
            return output.str();
        }
};

class Watson : public Character {
    private:
        int hp;
        int exp;

    public:
        string moving_rule;
        string str_moving_rule;

    public:
        Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) : moving_rule(moving_rule), hp(init_hp), exp(init_exp) {
            CheckHpExp(hp, exp);
            this->name = "Watson";
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->str_moving_rule = moving_rule;
        }

        Position getNextPosition() {
            int r = pos.getRow();
            int c = pos.getCol();

            Position next_pos = MovingRule(pos, moving_rule);

            if (map->isValid(next_pos, this)) {
                return next_pos;
            }
            else 
                return Position::npos;
        }

        Position getCurrentPosition() const {
            return this->pos;
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
            return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + str_moving_rule + "]";
        }
};

int ManhattanDistance(Position pos1, Position pos2);

Position CriminalNextPos(int u_distance, int d_distance, int l_distance, int r_distance, Position pos) ;


class Criminal : public Character {
    private:
        Sherlock * sherlock;
        Watson * watson;
        Position prev_pos;

    public:
        Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : sherlock(sherlock), watson(watson) {
            this->name = "Criminal";
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->prev_pos = init_pos;  
        }

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

        Position getCurrentPosition() const {
            return this->pos;
        }

        Position getPrevPosition() const {
            return this->prev_pos;
        }
        
        void move() {
            prev_pos = pos;
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
    friend class TestStudyInPink;
    private:
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
            // for (int i = 0; i < count; i++) {
            //     delete arr_mv_objs[i];
            // }
            delete [] arr_mv_objs;
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
                return this->arr_mv_objs[index];
            } else {
                return nullptr;
            }
        }

        int size() const {
            return count;
        }

        string str() const {
            string res = "";
            for (int i = 0; i < count; ++i)
                res += get(i)->str() + ";";
            if (!res.empty()) {
                res.pop_back();
            }
            return "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";" + res + "]";
        }
};

class Configuration {
    friend class TestStudyInPink;
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
        Configuration(const string & filepath) : map_num_cols(0), map_num_rows(0), max_num_moving_objects(0), num_fake_walls(0), num_walls(0), sherlock_init_exp(0), sherlock_init_hp(0), watson_init_exp(0), watson_init_hp (0), num_steps(0) {
            ifstream file(filepath);
            if (!file.is_open()) {
                cerr << "Error: cannot open file " << filepath << endl;
                return;
            }
                            
                            

            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                istringstream iss2(line);
                istringstream iss3(line);
                string key, value;
                char delim;
                int r, c;

                if(getline(iss2, key, '=')) {
                    if (key == "NUM_WALLS")
                        iss2 >> num_walls;
                    else if (key == "NUM_FAKE_WALLS")
                        iss2 >> num_fake_walls;
                }

                if(getline(iss3, key, '=')) {
                    if (key == "ARRAY_WALLS") {
                        int count = 0;
                        string check = "";
                        while (getline(iss3, key, ';')) {
                            check += key + " ";
                            count++;    
                        }
                        this->num_walls = count;
                    }

                    else if (key == "ARRAY_FAKE_WALLS") {
                        int count = 0;
                        string check = "";
                        while (getline(iss3, key, ';')) {
                            check += key + " ";
                            count++;
                        }
                        this->num_fake_walls = count;
                    }
                }

                if (getline(iss, key, '=')) {
                    if (key == "MAP_NUM_ROWS") 
                        iss >> map_num_rows;
                    else if (key == "MAP_NUM_COLS") 
                        iss >> map_num_cols;
                    else if (key == "MAX_NUM_MOVING_OBJECTS") 
                        iss >> max_num_moving_objects;
                    else if (key == "SHERLOCK_INIT_HP")
                        iss >> sherlock_init_hp;
                    else if (key == "SHERLOCK_INIT_EXP")
                        iss >> sherlock_init_exp;
                    else if (key == "WATSON_INIT_HP")
                        iss >> watson_init_hp;
                    else if (key == "WATSON_INIT_EXP")
                        iss >> watson_init_exp;
                    else if (key == "NUM_STEPS")
                        iss >> num_steps;
                    else if (key == "SHERLOCK_MOVING_RULE")
                        iss >> sherlock_moving_rule;
                    else if (key == "SHERLOCK_INIT_POS") {
                        iss >> delim >> r >> delim >> c;
                        sherlock_init_pos = Position(r,c);
                    }
                    else if (key == "WATSON_MOVING_RULE")
                        iss >> watson_moving_rule;
                    else if (key == "WATSON_INIT_POS") {
                        iss >> delim >> r >> delim >> c;
                        watson_init_pos = Position(r,c);
                    }
                    else if (key == "CRIMINAL_INIT_POS") {
                        iss >> delim >> r >> delim >> c;
                        criminal_init_pos = Position(r,c);
                    }

                    // else if (key == "ARRAY_WALLS") {
                    //     int count = 0;
                    //     string check = "";
                    //     while (getline(iss, key, ';')) {
                    //         check += key + " ";
                    //         count++;
                    //     }
                    //     this->num_walls = count;
                    //     arr_walls = new Position[num_walls];
                    //     string temp = check;
                    //     istringstream iss2(temp);
                    //     for (int i = 0; i < num_walls; ++i) {
                    //         iss2 >> delim >> delim >> r >> delim >> c;
                    //         Position pos = Position(r, c);
                    //         arr_walls[i] = pos;
                    //         if (i < num_walls - 1) { 
                    //             iss2.ignore();
                    //         }
                    //     }
                    // }

                    // else if (key == "ARRAY_FAKE_WALLS") {
                    //     int count = 0;
                    //     string check = "";
                    //     while (getline(iss, key, ';')) {
                    //         check += key + " ";
                    //         count++;
                    //     }
                    //     this->num_fake_walls = count;
                    //     arr_fake_walls = new Position[num_fake_walls];
                        
                    // }

                    else if (key == "ARRAY_WALLS") {
                        arr_walls = new Position[num_walls];
                        for (int i = 0; i < num_walls; ++i) {
                            iss >> delim >> delim >> r >> delim >> c;
                            Position pos = Position(r, c);
                            arr_walls[i] = pos;
                            if (i < num_walls - 1) { 
                                iss.ignore();
                            }
                        }
                    }
                    
                    else if (key == "ARRAY_FAKE_WALLS") {
                        arr_fake_walls = new Position[num_fake_walls];
                        for (int i = 0; i < num_fake_walls; ++i) {
                            iss >> delim >> delim >> r >> delim >> c;
                            Position pos = Position(r, c);
                            arr_fake_walls[i] = pos;
                            if (i < num_fake_walls - 1) { 
                                iss.ignore();
                            }
                        }
                    }
                }
            }
            file.close();
        }
        

        ~Configuration() {
            // delete arr_walls;
            // delete arr_fake_walls;
        }

        string str() const {
            ostringstream oss;
            oss << "Configuration[" << endl;
            oss << "MAP_NUM_ROWS=" << map_num_rows << endl;
            oss << "MAP_NUM_COLS=" << map_num_cols << endl;
            oss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
            oss << "NUM_WALLS=" << num_walls << endl;
            oss << "ARRAY_WALLS=[";
            if (num_walls == 0)
                oss << "]" << endl;
            else 
                for (int i = 0; i < num_walls; ++i) {
                    if (i == num_walls - 1) 
                        oss << arr_walls[i].str() << "]" << endl;
                    else
                        oss << arr_walls[i].str() << ";";
                }
            oss << "NUM_FAKE_WALLS=" << num_fake_walls << endl;           
            oss << "ARRAY_FAKE_WALLS=[";
            if (num_fake_walls == 0)
                oss << "]" << endl;
            else 
                for (int i = 0; i < num_fake_walls; ++i) {
                    if (i == num_fake_walls - 1) 
                        oss << arr_fake_walls[i].str() << "]" << endl;
                    else
                        oss << arr_fake_walls[i].str() << ";";
                }
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
    friend class TestStudyInPink;
    public:
        RobotType type;
        BaseItem * item;
    
    public:
        Robot(RobotType in_type) : type(in_type) {}
};

class RobotC : public Robot {
    public:
        Criminal * criminal;

    public:
        RobotC(int index, const Position & init_pos, Map * map, Criminal * criminal) : Robot(C) {
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->name = "C";
            this->criminal = criminal;
        }

        Position getNextPosition() {
            Position next_pos = criminal->getPrevPosition();
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
            return "Robot[pos=" + pos.str() + ";type=" + this->name  + ";dist=]";
        }
};

Position RobotSMovingRule(int u_distance, int d_distance, int l_distance, int r_distance, Position pos);

class RobotS : public Robot {
    public:
        Sherlock * sherlock;

    public:
        RobotS(int index, const Position & init_pos, Map * map, Sherlock * sherlock) : Robot(S), sherlock(sherlock) {
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->name = "S";
        }

        ~RobotS() {}

        Position getNextPosition() {
            Position next_pos = this->pos;

            Position Upos = this->pos;
            Upos.setRow(Upos.getRow() - 1);
            int Udistance = ManhattanDistance(Upos, sherlock->getCurrentPosition());

            Position Dpos = this->pos;
            Dpos.setRow(Dpos.getRow() + 1);
            int Ddistance = ManhattanDistance(Dpos, sherlock->getCurrentPosition());

            Position Lpos = this->pos;
            Lpos.setCol(Lpos.getCol() - 1);
            int Ldistance = ManhattanDistance(Lpos, sherlock->getCurrentPosition());

            Position Rpos = this->pos;
            Rpos.setCol(Rpos.getCol() + 1);
            int Rdistance = ManhattanDistance(Rpos, sherlock->getCurrentPosition());

            next_pos = RobotSMovingRule(Udistance, Ddistance, Ldistance, Rdistance, pos);

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
            return "Robot[pos=" + pos.str() + ";type=" + this->name + ";dist=" + to_string(ManhattanDistance(pos, sherlock->getCurrentPosition())) + "]";
        }
};

class RobotW : public Robot {
    public:
        Watson * watson;

    public:
        RobotW(int index, const Position & init_pos, Map * map, Watson * watson) : Robot(W), watson(watson) {
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->name = "W";
        }

        ~RobotW() {}

        Position getNextPosition() {
            Position next_pos = this->pos;

            Position Upos = this->pos;
            Upos.setRow(Upos.getRow() - 1);
            int Udistance = ManhattanDistance(Upos, watson->getCurrentPosition());

            Position Dpos = this->pos;
            Dpos.setRow(Dpos.getRow() + 1);
            int Ddistance = ManhattanDistance(Dpos, watson->getCurrentPosition());

            Position Lpos = this->pos;
            Lpos.setCol(Lpos.getCol() - 1);
            int Ldistance = ManhattanDistance(Lpos, watson->getCurrentPosition());

            Position Rpos = this->pos;
            Rpos.setCol(Rpos.getCol() + 1);
            int Rdistance = ManhattanDistance(Rpos, watson->getCurrentPosition());

            next_pos = RobotSMovingRule(Udistance, Ddistance, Ldistance, Rdistance, pos);

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
            return "Robot[pos=" + pos.str() + ";type=" + to_string(type) + ";dist=" + to_string(ManhattanDistance(pos, watson->getCurrentPosition())) + "]";
        }
};

Position RobotSW_distance(Sherlock * sherlock, Watson * watson, Position pos) ;

class RobotSW : public Robot {
    public:
        Sherlock * sherlock;
        Watson * watson;

    public:
        RobotSW(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : Robot(SW), sherlock(sherlock), watson(watson) {
            this->pos = init_pos;
            this->map = map;
            this->index = index;
            this->name = "SW";
        }

        ~RobotSW() {}

        Position getNextPosition() {
            Position next_pos = this->pos;
            next_pos = RobotSW_distance(sherlock, watson, pos);
            next_pos = RobotSW_distance(sherlock, watson, next_pos);

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
            return "Robot[pos=" + pos.str() + ";type=" + to_string(type) + ";dist=" + to_string(ManhattanDistance(pos, sherlock->getCurrentPosition()) + ManhattanDistance(pos, watson->getCurrentPosition())) + "]";
        }
};

class BaseItem {
    friend class TestStudyInPink;
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
                if (challenge != to_string(robot->type)) {
                    dynamic_cast<Watson*>(obj)->setExp(dynamic_cast<Watson*>(obj)->getExp() - 50);
                }
            }
        }
};

class Item {
public:
    BaseItem* item;
    Item* next;

    Item(BaseItem* item) : item(item), next(nullptr) {}
};

class BaseBag {
    friend class TestStudyInPink;
    public:
        Character * obj;
        int capacity;
        int count;
        Item * head;

    public:
        BaseBag(Character * obj, int capacity) : obj(obj), capacity(capacity), count(0), head(nullptr) {}

        ~BaseBag() {
            Item* current = head;
            while (current != nullptr) {
                Item* nextItem = current->next;
                delete current;
                current = nextItem;
            }
        }

        bool isFull() const {
            return count == capacity;
        }

        virtual bool insert(BaseItem* item) {
            if (isFull()) {
                return false;
            }
            Item* newItem = new Item(item);
            newItem->next = head;
            head = newItem;
            count++;
            return true;
        }

        virtual BaseItem* get() {
            Item* current = head;
            Item* prev = nullptr;
            while (current != nullptr) {
                if (current->item->canUse(obj, nullptr)) {
                    if (prev == nullptr) {
                        head = current->next;
                    } else {
                        prev->next = current->next;
                        current->next = head;
                        head = current;
                        current = head;
                        head = head->next;
                    }
                    BaseItem* data = current->item;
                    delete current;
                    return data;
                }
                prev = current;
                current = current->next;
            }
            return nullptr;
        }

        virtual BaseItem * get(ItemType type) {
            Item* current = head;
            Item* prev = nullptr;
            while (current != nullptr) {
                if (current->item->getType() == type) {
                    if (prev == nullptr) {
                        head = current->next;
                    } else {
                        prev->next = current->next;
                        current->next = head;
                        head = current;
                        current = head;
                        head = head->next;
                    }
                    BaseItem* data = current->item;
                    delete current;
                    return data;
                }
                prev = current;
                current = current->next;
            }
            return nullptr;
        }

        int size() const {
            return count;
        }

        virtual string str() const {
            string res = "";
            Item * current = head;
            for (int i = 0; i < count; ++i) {
                res += current->item->getType() + ",";
                current = current->next;
            }
            if (!res.empty()) {
                res.pop_back();
            }
            return "Bag[count=" + to_string(count) + ";" + res + "]";
        }
};

class SherlockBag : public BaseBag {
    public:
        SherlockBag(Sherlock * sherlock) : BaseBag(sherlock, 13) {}
};

class WatsonBag : public BaseBag {
    public:
        WatsonBag(Watson * watson) : BaseBag(watson, 15) {}
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
        StudyPinkProgram(const string & config_file_path) {
            config = new Configuration(config_file_path);
            map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
            sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
            watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
            criminal = new Criminal(2, config->criminal_init_pos, map, sherlock, watson);
            arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
            arr_mv_objs->add(criminal);
            arr_mv_objs->add(sherlock);
            arr_mv_objs->add(watson);
        }

        bool isStop() const {
            if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
                return true;
            }
            else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
                return true;
            }
            else if (sherlock->getHp() <= 0 || watson->getHp() <= 0) {
                return true;
            }
            return false;
        }

        void printResult() const {
            if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
                cout << "Sherlock caught the criminal" << endl;
            }
            else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(), criminal->getCurrentPosition().getCol())) {
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

        ~StudyPinkProgram() {
            delete config;
            delete sherlock;
            delete watson;
            delete criminal;
            delete map;
            delete arr_mv_objs;
        }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */