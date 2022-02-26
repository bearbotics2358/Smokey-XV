#include <math.h>
#include "Autonomous.h"





Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Joystick *ButtonBox, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector):
    a_Gyro(Gyro),
    a_ButtonBox(ButtonBox),
    a_SwerveDrive(SwerveDrive),
    a_Anticipation(),
    a_BallShooter(BallShooter),
    a_Collector(Collector),
    a_AutoState0(kAutoIdle0),
    a_AutoState1(kAutoIdle1),
    a_AutoState2(kAutoIdle2),
    a_AutoState3(kAutoIdle3),
    a_AutoState4(kAutoIdle4),
    a_AutoState5(kAutoIdle5) 
    
{

    autoPathMaster = -1;
    BallsShot = 0;
    prevbeam = false;
    currbeam = true;
    limeangle = 0.0;
    drivestart = 0.0;
    look = true;

}

void Autonomous::Init(){
	// a_Gyro->Zero();
    a_Anticipation.Start();
}

void Autonomous::DecidePath(){
    
    if(a_ButtonBox->GetRawButton(1)){
    
        autoPathMaster = 0;

    }
    else if(a_ButtonBox->GetRawButton(2)){
    
        autoPathMaster = 1;

    }



}



void Autonomous::DecidePath(int intent){
    
    autoPathMaster = intent;

}

int Autonomous::GetCurrentPath(){
    
    return autoPathMaster;
}

void Autonomous::StartPathMaster(){
	
    switch(autoPathMaster){
		
        case -1:
			// Error!
			frc::SmartDashboard::PutBoolean("Auto Started", false);
		
        	break;
		
        case 0:
			frc::SmartDashboard::PutBoolean("Auto Started", true);
			AutonomousStart0();
		
        	break;
		
        case 1:
			frc::SmartDashboard::PutBoolean("Auto Started", true);
			AutonomousStart1();
		
        	break;
	}
}

void Autonomous::StartPathMaster(int path){
	
    switch(path){
		
        case -1:
			// Error!
			frc::SmartDashboard::PutBoolean("Auto Started", false);
		
        	break;
		
        case 0:
			frc::SmartDashboard::PutBoolean("Auto Started", true);
			AutonomousStart0();
		
        	break;
		
        case 1:
			frc::SmartDashboard::PutBoolean("Auto Started", true);
			AutonomousStart1();
		
        	break;
	}
}

void Autonomous::PeriodicPathMaster(){
	switch(autoPathMaster){
		case -1:
			// Error!
			break;
		case 0:
			AutonomousPeriodic0();

			break;

		case 1:
			AutonomousPeriodic1();
			
            break;
	}
}

void Autonomous::PeriodicPathMaster(int path){
	switch(path){
		case -1:
			// Error!
			break;
		case 0:
			AutonomousPeriodic0();

			break;
		
        case 1:
			AutonomousPeriodic1();
			
            break;

	}
}




void Autonomous::AutonomousStart0(){

   // a_AutoState0 = kArmMove0;
    a_Gyro->Zero();

}

// ----------------------------------AUTONOMOUS ROUTINES---------------------------------------- //

void Autonomous::AutonomousPeriodic0(){

    AutoState0 nextState = a_AutoState0;

	switch(a_AutoState0){

	    case kAutoIdle0:
		    IDontLikeExercise();

		    break;

        case kDriveAway0:
            DriveDist(120, 0);

            break;
    }
}

void Autonomous::AutonomousStart1(){
    
    a_Gyro->Zero();

}


void Autonomous::AutonomousPeriodic1(){

    AutoState1 nextState = a_AutoState1;

	switch(a_AutoState1){

	    case kAutoIdle1:
		    IDontLikeExercise();
        
		    break;

        case kShoot1:
            a_BallShooter->setSpeed(2200);
            
            if(a_BallShooter->getSpeed() >= 2200) 
            {
                a_Collector->setIndexerMotorSpeed(300);
            }
            
            break;

        case kTaxi1:
            DriveDist(120, 180);

            break;
    }
}


void Autonomous::IDontLikeExercise(){

    a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
    a_Collector->setCollectorMotorSpeed(0);
    a_Collector->setIndexerMotorSpeed(0);
    a_BallShooter->setSpeed(0);
    
}

/*
void Autonomous::waitplz(double anticipate){
    double woah = frc2::Timer::GetMatchTime().to<double>;
    
    while (a_Anticipation.Get().to<double> < woah.to<double> + anticipate)
    {
        if (woah >= 15){
            break; 
        }
    }
    


}
*/

bool Autonomous::DriveDist(double dist, double angle){ // true is done, false is not done

    if((double) fabs(a_SwerveDrive->getAvgDistance()) < dist){
        a_SwerveDrive->driveDistance(dist, angle);
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;


    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;

    }
            

}


/*bool Autonomous::RootyTootyShooty(int count, float vel){
    currbeam = CheckBallPos();
    
    
    if(BallsShot < ((2 * count)) + 1 && currbeam != prevbeam){
        BallsShot++;
        prevbeam = currbeam;
        return false;
    }
    else if(BallsShot < ((2 * count)) + 1){
        a_CFS->ShootVelocity(vel);
        float avg = (fabs(a_CFS->GetWheelSpeedL()) + fabs(a_CFS->GetWheelSpeedR())) / 2.0;
        if(avg >= 400)
        {
            a_CFS->FeedVelocity(vel);
        }
        else
        {
            a_CFS->FeedVelocity(0); 
        }
        
        return false;
    }
    else{
        BallsShot = 0;
        a_CFS->ShootVelocity(0);
        a_CFS->FeedVelocity(0);
        return true;
    }


    
}
*/

bool Autonomous::TurnTaAngle(float angle){
    
    if(fabs(a_Gyro->GetAngle(0) - angle) >= 1){
        a_SwerveDrive->turnToAngle(a_Gyro->GetAngle(0), angle);
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;


    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;

    }

}


bool Autonomous::IHaveAProposal(float speed, float dir, float dist){ // true is done, false is not done

    if(fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)){

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))){
		    a_SwerveDrive->GoToTheDon(speed / 2, dir, dist, a_Gyro->GetAngle(0));
            a_BallShooter->setSpeed(AUTO_SHOOT_VELOCITY);
		} else {
            a_SwerveDrive->GoToTheDon(speed, dir, dist, a_Gyro->GetAngle(0));
            a_BallShooter->setSpeed(0);
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;

    }
            

}


bool Autonomous::GoToMcDonalds(float speed, float dir, float dist){ // true is done, false is not done

    if(fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)){

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))){
		    a_SwerveDrive->GoToTheDon(speed / 2, dir, dist, a_Gyro->GetAngle(0));
            //a_CFS->ShootVelocity(AUTO_SHOOT_VELOCITY);
            //a_CFS->AutoCollect();
		} else {
            a_SwerveDrive->GoToTheDon(speed, dir, dist, a_Gyro->GetAngle(0));
            //a_CFS->AutoCollect();
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;

    }
            

}