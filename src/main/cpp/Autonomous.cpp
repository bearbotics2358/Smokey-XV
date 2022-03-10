#include <math.h>
#include "Autonomous.h"


Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector, BeamBreak *BeamBreak):
    a_Gyro(Gyro),
    a_Timer(Timer),
    a_Joystick(Joystick),
    a_SwerveDrive(SwerveDrive),
    a_BallShooter(BallShooter),
    a_Collector(Collector),
    a_BeamBreak(BeamBreak),
    a_AutoState0(kAutoIdle0),
    a_AutoState1(kAutoIdle1),
    a_AutoState2(kAutoIdle2),
    a_AutoState3(kAutoIdle3),
    a_AutoState4(kAutoIdle4),
    a_AutoState5(kAutoIdle5)
    
{

    autoPathMaster = 1;
    BallsShot = 0;
    prevbeam = false;
    currbeam = true;
    limeangle = 0.0;
    drivestart = 0.0;
    look = true;

}

void Autonomous::Init(){
	a_Gyro->Zero();
}

void Autonomous::DecidePath(){
    
    if(a_Joystick->GetRawButton(11)){
    
        autoPathMaster = 0;

    }
    else if(a_Joystick->GetRawButton(10)){
    
        autoPathMaster = 1;

    }

    else if(a_Joystick->GetRawButton(12)){
        
        autoPathMaster = 2;
    
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
			frc::SmartDashboard::PutBoolean("Auto Failed", false);
		
        	break;
		
        case 0:
			frc::SmartDashboard::PutBoolean("0-ball Taxi started", true);
			AutonomousStart0();
		
        	break;
		
        case 1:
			frc::SmartDashboard::PutBoolean("1-ball Taxi started", true);
			AutonomousStart1();
		
        	break;

        case 2:
            frc::SmartDashboard::PutBoolean("2-ball Taxi started", true);
            AutonomousStart2();
	}
}

void Autonomous::StartPathMaster(int path){
	
    switch(path){
		
        case -1:
			// Error!
			frc::SmartDashboard::PutBoolean("Auto Failed", false);
		
        	break;
		
        case 0:
			frc::SmartDashboard::PutBoolean("0-ball Taxi Started", true);
			AutonomousStart0();
		
        	break;
		
        case 1:
			frc::SmartDashboard::PutBoolean("1-ball Taxi Started", true);
			AutonomousStart1();
		
        	break;
        
        case 2:
            frc::SmartDashboard::PutBoolean("2-ball Taxi Started", true);
            AutonomousStart2();

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

        case 2:
            AutonomousPeriodic2();

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

        case 2:
            AutonomousPeriodic2();

            break;
	}
}


// ----------------------------------AUTONOMOUS ROUTINES---------------------------------------- //

void Autonomous::AutonomousStart0(){

    a_AutoState0 = kDriveAway0;
    a_Gyro->Zero();

}

void Autonomous::AutonomousPeriodic0(){

    AutoState0 nextState = a_AutoState0;

	switch(a_AutoState0){

	    case kAutoIdle0:
		    IDontLikeExercise();

		    break;

        case kDriveAway0:
            if(IHaveAProposal(1000, 35, 0)){
                nextState = kAutoIdle0;
            }
            break;
    }
    a_AutoState0 = nextState;
}

void Autonomous::AutonomousStart1(){
    
    a_AutoState1 = kShoot1;
    a_Gyro->Zero();

}


void Autonomous::AutonomousPeriodic1(){

    AutoState1 nextState = a_AutoState1;

	switch(a_AutoState1){

	    case kAutoIdle1:
		    IDontLikeExercise();
            break;

        case kShoot1:
            if(IndexAndShoot(SHOOT_FROM_WALL)){
                nextState = kStartTimer1;
            }
            break;

        case kStartTimer1:
            a_Timer->Start();
            nextState = kCheckShot1;
            break;

        case kCheckShot1:
            if(WaitForTime(1)){
                nextState = kDoneShooting1;
            }

            break;

        case kDoneShooting1:
            IDontLikeExercise();
            nextState = kTaxi1;
            break;
        
        case kTaxi1:
            if(DriveDist(-120, 0)){
                nextState = kAutoIdle1;
            }
            break;
    }
    a_AutoState1 = nextState;
}

void Autonomous::AutonomousStart2(){
    a_AutoState2 = kCollectDown2;
    a_Gyro->Zero();
}

void Autonomous::AutonomousPeriodic2(){

    AutoState2 nextState = a_AutoState2;

    switch (a_AutoState2){
        case kAutoIdle2:
            IDontLikeExercise();
            
            break;
        
        case kCollectDown2:
            ToggleCollector();
            nextState = kDriveBackThroughBall2;
            break;

        case kDriveBackThroughBall2:
            if(GoToMcDonalds(0.4, 0, 36)){
                nextState = kLoad2;
            }
            break;

        case kLoad2:
            ToggleCollector();
            nextState = kTurn2;
            break;
        
        case kTurn2:
            if(TurnTaAngle(-159)){
                nextState = kSpool2;
            }
            break;
        
        case kSpool2:
            SpoolShooter(SHOOT_FROM_WALL);
            nextState = kDriveToWall2;
            break;

        case kDriveToWall2:
            if(IHaveAProposal(0.3, 120, 0)){
                nextState = kShoot2;
            }
            break;

        case kShoot2:
            if(IndexAndShoot(SHOOT_FROM_WALL)){
                nextState = kCheckFirstShot2;
            }
            break;
       
        case kCheckFirstShot2:
            if(WaitForTime(1)){
                nextState = kSecondShoot2;
            }
            break;

        case kSecondShoot2:
            if(IndexAndShoot(SHOOT_FROM_WALL)){
                nextState = kCheckSecondShot2;
            }
            break;
        
        case kCheckSecondShot2:
            if(WaitForTime(1)){
                nextState = kAutoIdle2;
            }
            break;

    }
    a_AutoState2 = nextState;
}

void Autonomous::IDontLikeExercise(){

    a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
    a_Collector->setCollectorMotorSpeed(0);
    a_Collector->setIndexerMotorSpeed(0);
    a_BallShooter->setSpeed(0);
    
}

void Autonomous::StartTimer(){
    a_Timer->Start();
}

bool Autonomous::WaitForTime(double time){ //-----MUST HAVE A SEPARATE CASE USING TIMERSTART-----//
    

    if (a_Timer->Get().value() < time){
        return false;
    }
    else if (a_Timer->Get().value() >= time){
        return true;
        a_Timer->Stop();
    }

}


void Autonomous::SpoolShooter(float speed) {
    a_BallShooter->setSpeed(speed);
}

void Autonomous::ToggleCollector() {
    a_Collector->toggleSolenoid();
}

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

bool Autonomous::IndexAndShoot(float speed){ //returns true if the shooter is running correctly and the indexer has switched on
    a_BallShooter->setSpeed(speed);

    if(a_BallShooter->getSpeed() >= speed - 200) {
        a_Collector->setIndexerMotorSpeed(COLLECTOR_MOTOR_RPM);
        return true;
    }
    else if(a_BallShooter->getSpeed() < speed - 200) {
        return false;
    }
}

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

bool Autonomous::BallShot(float speed){ //looks for a dip in RPM value to detect a shot being made
    //going to reimplement using beambreak soon
    if(a_BeamBreak->beamBroken()){
        a_Collector->setIndexerMotorSpeed(0);
        return true;
    }
    else if(a_BallShooter->getSpeed() >= speed){
        return false;
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
            a_Collector->setCollectorMotorSpeed(300);
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->GetAngle(0), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;

    }
            

}