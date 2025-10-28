#include "memberModel.hpp"

// MEMBER DATA

MemberData::MemberData(double length,
                       cs_type::cross_section_type type, 
                       Materials* material)
{
    switch(type)
    {
        case cs_type::RECTANGLE_WITH_CIRCLE_OUT:
        this->crossSection = new RectangleWithCircleCut();
        break;
        
        default:
        log(WARNING, "Undefined cross section area type. Defaulting to rectangle with a circle cut out.");
        this->crossSection = new RectangleWithCircleCut();
    }
    
    this->material = material;
    this->id = (unsigned)mh_random(0, MAX_ID);
    this->set_length(length);

    this->jointIDAtStart = -1;
    this->jointIDAtEnd = -1;
}

MemberData::MemberData(const MemberData& obj)
{
    this->id = obj.id;

    this->length = obj.length;
    this->crossSection = obj.crossSection->clone();
    this->material = obj.material->clone();

    this->axialForce = obj.axialForce;

    this->jointIDAtStart = obj.jointIDAtStart;
    this->jointIDAtEnd = obj.jointIDAtEnd;
}

MemberData& MemberData::operator=(MemberData obj)
{
    this->swap(*this, obj);
    return *this;
}

MemberData::~MemberData()
{
    delete this->crossSection;
    delete this->material;
}

void MemberData::swap(MemberData& obj1, MemberData& obj2)
{
    std::swap(obj1.id, obj2.id);

    std::swap(obj1.length, obj2.length);
    std::swap(obj1.crossSection, obj2.crossSection);
    std::swap(obj1.material, obj2.material);

    std::swap(obj1.axialForce, obj2.axialForce);

    std::swap(obj1.jointIDAtStart, obj2.jointIDAtStart);
    std::swap(obj1.jointIDAtEnd, obj2.jointIDAtEnd);
}

unsigned int MemberData::get_id() const
{
    return this->id;
}

double MemberData::get_length() const
{
    return this->length;
}

const CrossSection* MemberData::get_cross_section() const
{
    return this->crossSection;
}

CrossSection* MemberData::cross_section()
{
    return this->crossSection;
}

const Materials* MemberData::get_material() const
{
    return this->material;
}

int MemberData::get_joint_id_at_start() const
{
    return this->jointIDAtStart;
}

int MemberData::get_joint_id_at_end() const
{
    return this->jointIDAtEnd;
}

double MemberData::get_axial_force() const
{
    return this->axialForce;
}

bool MemberData::set_length(double newLength)
{
    if(newLength < this->crossSection->get_girth())
    {
        this->length = this->crossSection->get_girth()+0.01;
        return false;
    }

    this->length = newLength;
    return true;
}

void MemberData::set_axial_force(double axialForce)
{
    this->axialForce = axialForce;
}

void MemberData::set_joint_id_at_start(int id)
{
    this->jointIDAtStart = id;
}

void MemberData::set_joint_id_at_end(int id)
{
    this->jointIDAtEnd = id;
}

bool MemberData::is_yielded()
{
    double maxForce = this->crossSection->get_area();
    if(this->axialForce > 0)
    {
        maxForce *= this->material->get_tensile_strength();
    }
    else 
    {
        maxForce *= this->material->get_compressive_strength();
    }
    return maxForce < abs(this->axialForce);
}

bool MemberData::is_buckled()
{
    if(this->axialForce < 0)
    {
        double I = this->crossSection->get_I();
        double Leff = this->length;
        double E = this->material->get_young_modulus();

        double maxForce = (pow(PI,2)*E*I) / pow(Leff,2);
        
        return maxForce < -this->axialForce;
    }
    return false;
}

// MEMBER MODEL

// PRIVATE
void MemberModel::set_angle(double newAngle)
{
    this->angle = std::fmod(newAngle, 360.0);
    this->angle += (this->angle < 0.0 ? 360.0 : 0);
    
    this->counter_angle = std::fmod(180 + this->angle, 360.0);
}

void MemberModel::set_counter_angle(double newAngle)
{
    this->counter_angle = std::fmod(newAngle, 360.0);
    this->counter_angle += (this->counter_angle < 0.0 ? 360.0 : 0);

    this->angle = std::fmod(180 + this->counter_angle, 360.0);
}

bool MemberModel::set_scaled_length(double newLength)
{
    bool status = this->properties.set_length(newLength);
    this->scaledLength = this->properties.get_length() * this->scale;
    return status;
}

void MemberModel::update_mid_pos()
{
    vector_2d middle = vector_subtract(this->endPos, this->startPos);
    middle = { middle.x/2, middle.y/2 };
    this->midPos = vector_add(this->startPos, middle);
}

// PUBLIC

MemberModel::MemberModel(double length, double scale, cs_type::cross_section_type type, Materials* material)
{
    this->properties = MemberData(length, type, material);
    this->scale = scale;

    this->startPos = { 0.0, 0.0 };
    this->midPos = { 0.0, 0.0 };
    this->endPos = { 0.0, 0.0 };;

    this->set_angle(0);
    this->modify_length(length);
}

double MemberModel::get_angle() const
{
    return this->angle;
}

double MemberModel::get_counter_angle() const
{
    return this->counter_angle;
}

vector_2d MemberModel::get_start_pos() const
{
    return this->startPos;
}

vector_2d MemberModel::get_mid_pos() const
{
    return this->midPos;
}

vector_2d MemberModel::get_end_pos() const
{
    return this->endPos;
}

MemberData& MemberModel::get_properties()
{
    return this->properties;
}

double MemberModel::get_scale() const
{
    return this->scale;
}

double MemberModel::get_id() const
{
    return this->properties.get_id();
}

void MemberModel::set_scale(double newScale)
{
    this->scale = newScale;
    this->modify_length(this->properties.get_length());
}

// Modifying length that changes both start's and end's pos.
void MemberModel::modify_length(double newLength)
{
    this->set_scaled_length(newLength);

    double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
    double deltaLength = this->scaledLength - oldLength;

    vector_2d direction = vector_from_angle(this->angle, 1);

    // change start's position
    vector_2d toEndDirection = { -direction.x * deltaLength / 2, -direction.y * deltaLength / 2 };
    this->startPos = vector_add(this->startPos, toEndDirection);

    // change end's position
    vector_2d toStartDirection = { direction.x * deltaLength / 2, direction.y * deltaLength / 2 };
    this->endPos = vector_add(this->endPos, toStartDirection);
}

// Modifying length that only changes end point. Start's position remains the same
void MemberModel::modify_length_on_start(double newLength)
{
    this->set_scaled_length(newLength);

    double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
    double deltaLength = this->scaledLength - oldLength;

    vector_2d direction = vector_from_angle(this->angle, 1);
    direction = { direction.x * deltaLength, direction.y * deltaLength };
    this->endPos = vector_add(this->endPos, direction);
}

// Modifying length that only changes start point. End's position remains the same
void MemberModel::modify_length_on_end(double newLength)
{
    this->set_scaled_length(newLength);

    double oldLength = vector_magnitude(vector_subtract(this->startPos, this->endPos));
    double deltaLength = this->scaledLength - oldLength;

    vector_2d direction = vector_from_angle(this->angle, 1);
    direction = { -direction.x * deltaLength, -direction.y * deltaLength };
    this->startPos = vector_add(this->startPos, direction);
}

void MemberModel::modify_start_pos(double x, double y)
{
    this->startPos = { x, y };
    vector_2d distance = vector_subtract(this->startPos, this->endPos);

    // change on the end side
    this->set_counter_angle(vector_angle(distance));
    this->set_scaled_length(vector_magnitude(distance) / this->scale);
    this->modify_length_on_end(this->properties.get_length());

    this->update_mid_pos();
}

void MemberModel::modify_end_pos(double x, double y)
{
    this->endPos = { x, y };
    vector_2d distance = vector_subtract(this->endPos, this->startPos);

    // change on the start side
    this->set_angle(vector_angle(distance));
    this->set_scaled_length(vector_magnitude(distance) / this->scale);
    this->modify_length_on_start(this->properties.get_length());

    this->update_mid_pos();
}

void MemberModel::modify_mid_pos(double x, double y)
{
    vector_2d destination = vector_2d{ x, y };
    vector_2d direction = vector_subtract(destination, this->midPos);

    this->startPos = vector_add(direction, this->startPos);
    this->midPos = vector_add(direction, this->midPos);
    this->endPos = vector_add(direction, this->endPos);
}

void MemberModel::modify_angle(double angle)
{
    this->set_angle(angle);

    vector_2d direction = vector_subtract(this->endPos, this->startPos);
    double currentAngle = vector_angle(direction);
    vector_2d newDirection = matrix_multiply(rotation_matrix(this->angle - currentAngle), direction);

    this->endPos = vector_add(newDirection, this->startPos);
    this->update_mid_pos();
}

void MemberModel::modify_counter_angle(double angle)
{
    this->set_counter_angle(angle);

    vector_2d direction = vector_subtract(this->startPos, this->endPos);
    double currentAngle = vector_angle(direction);
    vector_2d newDirection = matrix_multiply(rotation_matrix(this->counter_angle - currentAngle), direction);

    this->startPos = vector_add(newDirection, this->endPos);
    this->update_mid_pos();
}

// detect point on start
bool MemberModel::is_intersect_start(double x, double y) const
{
    point_2d point = { x, y };

    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;
    double radius = scaledGirth / 2;
    circle startCircle = { this->startPos.x, this->startPos.y, radius };

    return point_in_circle(point, startCircle);
}

// detect point on end
bool MemberModel::is_intersect_end(double x, double y) const
{
    point_2d point = { x, y };

    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;
    double radius = scaledGirth / 2;
    circle endCircle = { this->endPos.x, this->endPos.y, radius };

    return point_in_circle(point, endCircle);
}

// detect circle on start
bool MemberModel::is_intersect_start(double x, double y, double radius) const
{
    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;
    return circles_intersect(
        x, y,
        radius,

        this->startPos.x, this->startPos.y,
        scaledGirth / 2
    );
}

// detect circle on end
bool MemberModel::is_intersect_end(double x, double y, double radius) const
{
    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;
    return circles_intersect(
        x, y,
        radius,

        this->endPos.x, this->endPos.y,
        scaledGirth / 2
    );
}

// Add intersection detection on body
bool MemberModel::is_intersect_body(double x, double y) const
{
    // TODO: might need to somehow rewrite this to speed it up. i.e. save this on some variables or something
    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;
    vector_2d line = vector_subtract(this->endPos, this->startPos);
    vector_2d direction = { line.x / this->scaledLength, line.y / this->scaledLength };

    vector_2d topDirection = matrix_multiply(rotation_matrix(-90), direction);
    topDirection = { topDirection.x * scaledGirth / 2, topDirection.y * scaledGirth / 2 };

    vector_2d left_corner_top = vector_add(topDirection, this->startPos);
    vector_2d left_corner_bot = vector_add({ -topDirection.x, -topDirection.y }, this->startPos);

    vector_2d right_corner_top = vector_add(left_corner_top, line);
    vector_2d right_corner_bot = vector_add(left_corner_bot, line);

    quad q;
    q.points[0] = { left_corner_top.x, left_corner_top.y };
    q.points[1] = { right_corner_top.x, right_corner_top.y };
    q.points[2] = { left_corner_bot.x, left_corner_bot.y };
    q.points[3] = { right_corner_bot.x, right_corner_bot.y };

    return point_in_quad({ x, y }, q);
}

void MemberModel::draw(color memberColor)
{
    double scaledGirth = this->properties.get_cross_section()->get_girth() * this->scale;

    // Start's circle
    draw_circle(memberColor, this->startPos.x, this->startPos.y, scaledGirth / 2);

    // End's circle
    draw_circle(memberColor, this->endPos.x, this->endPos.y, scaledGirth / 2);

    vector_2d line = vector_subtract(this->endPos, this->startPos);
    vector_2d direction = { line.x / this->scaledLength, line.y / this->scaledLength };

    // -90 since the positive y screen is going down
    vector_2d topDirection = matrix_multiply(rotation_matrix(-90), direction);
    topDirection = { topDirection.x * scaledGirth / 2, topDirection.y * scaledGirth / 2 };

    vector_2d botDirection = matrix_multiply(rotation_matrix(90), direction);
    botDirection = { botDirection.x * scaledGirth / 2, botDirection.y * scaledGirth / 2 };

    // Top's line
    vector_2d topStart = vector_add(topDirection, this->startPos);
    vector_2d topEnd = vector_add(topStart, line);
    draw_line(memberColor, topStart.x, topStart.y, topEnd.x, topEnd.y);

    // Bottom's line
    vector_2d botStart = vector_add(botDirection, this->startPos);
    vector_2d botEnd = vector_add(botStart, line);
    draw_line(memberColor, botStart.x, botStart.y, botEnd.x, botEnd.y);
}
