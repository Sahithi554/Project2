/**
 * @file CanadianExperienceApp.h
 * @author Charles Owen
 * @author Anik Momtaz
 *
 * Main application class
 */

#ifndef CANADIANEXPERIENCEAPP_H
#define CANADIANEXPERIENCEAPP_H

/**
 * Main application class
 */
class CanadianExperienceApp : public wxApp {
public:
    bool OnInit() override;
    int OnExit() override;
};

#endif //CANADIANEXPERIENCEAPP_H
