use crate::{
    branches::GlobalBranches, command::CommandOpt, cond_stmt::NextState, depot::Depot,
    executor::Executor, fuzz_type::FuzzType, search::*, stats,
};
use rand::prelude::*;
use std::sync::{
    atomic::{AtomicBool, Ordering},
    Arc, RwLock,
};

use std::{
    process::{Command, Stdio}, 
    io::prelude::*,
    path::{Path, PathBuf},
};
use std::fs::OpenOptions;
use std::fs;
use std::fs::File;
use std::io::{self, BufRead};
use std::io::Write;
use std::{thread};
use std::time;
use shared_memory::*;
 
//AIBAR ADDED HERE 
//
//
//
//
pub fn writeNameStart(id: u64)->String{
    let newLine = "\n";
    let mut header: String = "/data/shm/".to_owned();
    if let Ok(lines) = read_lines("/data/infos/targetsInfo.txt"){
    let mut flag = false;
        for line in lines{
            if let Ok(l) = line{
                if(!flag) {
                    flag = true;
                }else{
                    let mut split = l.split(",");
                    let vec = split.collect::<Vec<&str>>();
                    let without_prefix = vec[0].trim_start_matches("0x");
                    if let Ok(id_from_file) = u64::from_str_radix(without_prefix, 16){
                        if(id == id_from_file){
                            let fName: &str = vec[2];
                            header.push_str(fName);
                            return header;
                        }
                    }
                }

            }
        }
    } else{
      println!("COULD NOT OEPN TARGETS INFO");
    }
    return "".to_owned();
}

pub fn getNameStart(cmpid: u32)->String{
    let newLine = "\n"; 
    if let Ok(lines) = read_lines("/parmesan/misc/workdir/build/cmp.map"){
        let mut flag = false;
        for line in lines{
            if let Ok(l) = line{
                if(!flag) {
                    flag = true;
                }else{
                    let mut split = l.split(",");
                    let vec = split.collect::<Vec<&str>>();
                    let int0: u32 = vec[0].parse().unwrap();
                    let int1: u64 = vec[1].parse().unwrap();
                    if(int0 == cmpid){
                        return writeNameStart(int1);
                    }
                }
                
            }
        }
    }else{
      println!("COULDNTOPEN CMP.MAP");  
    }
    return "".to_owned()
    //return "";
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

  
pub fn fuzz_loop(
    running: Arc<AtomicBool>,
    cmd_opt: CommandOpt,
    depot: Arc<Depot>,
    global_branches: Arc<GlobalBranches>,
    global_stats: Arc<RwLock<stats::ChartStats>>,
) {
    
    let search_method = cmd_opt.search_method;
    let mut executor = Executor::new(
        cmd_opt.clone(),
        global_branches.clone(),
        depot.clone(),
        global_stats.clone(),
    );

    let san_cmd_opt = cmd_opt.sanopt();
    debug!("san_cmd_opt: {:?}", san_cmd_opt);
    let mut sanitized_executor = Executor::new(
        san_cmd_opt,
        global_branches.clone(),
        depot.clone(),
        global_stats.clone(),
    );
    //---------
    //AIBAR CODE
    //--------
    //VARIABLES
    let mut skipShm = false;
    let mut printIt = true;
    let mut nameCopy: String = "".to_owned();
    let mut nameCopyForDung: String = "".to_owned();
    static mut totalNotReached: u64 = 0; 
    static mut localCounter: u64 = 0;
    //let mut loops = 1000000;
    let mut totalBinaryExecution: u64 = 0;
    let mut totalBinaryExecutionBucket: u32  = 0;
    let mut totalNotReachedBucket: u32=0;
  
    //CREATE SHM FILE FOR MAIN
    let main_name="/data/shm/main";
    let shmem_main = match ShmemConf::new().size(8).flink(main_name).create(){
      Ok(m)=>m,
      Err(ShmemError::LinkExists)=>ShmemConf::new().flink(main_name).open().unwrap(),
      Err(e)=>{eprintln!("unable to create or open shemm flink {}: {}", main_name,e); return;}
    };
    //END MY CODE
    //

    while running.load(Ordering::Relaxed) {
        //if(loops ==0) {break};
        let entry = match depot.get_entry() {
            Some(e) => e,
            None => break,
        };
        
        let mut cond = entry.0;
        let priority = entry.1;

        if priority.is_done() {
            break;
        }

        if cond.is_done() {
            depot.update_entry(cond);
            continue;
        }

        trace!("{:?}", cond);

        let belong_input = cond.base.belong as usize;

        //aibar deleted here

        let buf = depot.get_input_buf(belong_input);

        {
            let fuzz_type = cond.get_fuzz_type();
            if cond.is_target {
                info!("Using Sanopt Executor");
            } else {
                info!("Using normal Executor");
            }
            let mut cur_executor = if cond.is_target {&mut sanitized_executor} else {&mut executor};
            match fuzz_type {
                FuzzType::ExploreFuzz => {
                    // Avoid sanitized binary when exploring
                },
                 _ => {
                    cur_executor = &mut sanitized_executor;
                },
            }
            //----------
            //AIBAR CODE
            //----------
            //
            //
            //Focus on Targets
            if(cond.is_target){
              skipShm=false;
            }else{
              skipShm=true;
            }
            if(!skipShm && printIt){
              println!("Current cmpid - {} ", cond.base.cmpid);
              //   println!("Not target");
            }  
            if(printIt && cond.is_target){println!("TARGET!!");}
            // RETRIEVE THE name of the function      
            let mut fName: String = "".to_owned();
            
            if(!skipShm){
              fName = getNameStart(cond.base.cmpid);
            }
            nameCopy = fName.clone();
            nameCopyForDung = fName.clone(); 
            //CREATE SHM FOR FUNCTION F
            //skipShm=false; //IF FUNCTION NAME WAS FOUND
            
            let mut shmem =  ShmemConf::new().size(1).flink("/data/shm/error").create().unwrap();
            if(!skipShm)
            {
              shmem = match ShmemConf::new().size(8).flink(fName).create(){
                Ok(m)=>m,
                Err(ShmemError::LinkExists)=>ShmemConf::new().flink(nameCopy).open().unwrap(),
                Err(e)=>{
                    eprintln!("CMPID: {}!, Unable to create or open shemm flink {}: {}",cond.base.cmpid, nameCopy,e);
                    skipShm=true;
                    ShmemConf::new().size(1).flink("/data/shm/error").create().unwrap()
                }
              };
            } 
            // Inititalize function execution counter to 0 
            let raw_ptr = shmem.as_ptr();
            if(!skipShm){
              unsafe{
                for i in 0..4{
                  *raw_ptr.add(i) = 0;
                }
              }
            }
            // Initialize main execution counter to 0
            let raw_ptr_main = shmem_main.as_ptr();
            unsafe{
              for i in 0..4{
                *raw_ptr_main.add(i) = 0;
              }
            }
            // Initialize local counter (how many times binary was shooted)
            unsafe{localCounter = 0;}
            unsafe{cur_executor.localCounter = &mut localCounter;}
            //the next lines are for DUNG
            /*
            if(nameCopyForDung.chars().count() > 0){
              cur_executor.toAddFuncName = true;
            }else{
              cur_executor.toAddFuncName= false;
            }
            cur_executor.funcName = nameCopyForDung;
            */
            //endof lines for DUNG 
            let handler = SearchHandler::new(running.clone(), &mut cur_executor, &mut cond, buf);
            match fuzz_type {
                FuzzType::ExploreFuzz => {
                    if handler.cond.is_time_expired() {
                        handler.cond.next_state();
                    }
                    if handler.cond.state.is_one_byte() {
                        OneByteFuzz::new(handler).run();
                    } else if handler.cond.state.is_det() {
                        DetFuzz::new(handler).run();
                    } else {
                        match search_method {
                            SearchMethod::Gd => {
                                GdSearch::new(handler).run(&mut thread_rng());
                            },
                            SearchMethod::Random => {
                                RandomSearch::new(handler).run();
                            },
                            SearchMethod::Cbh => {
                                CbhSearch::new(handler).run();
                            },
                            SearchMethod::Mb => {
                                MbSearch::new(handler).run();
                            },
                        }
                    }
                },
                FuzzType::ExploitFuzz => {
                    if handler.cond.state.is_one_byte() {
                        let mut fz = OneByteFuzz::new(handler);
                        fz.run();
                        fz.handler.cond.to_unsolvable(); // to skip next time
                    } else {
                        ExploitFuzz::new(handler).run();
                    }
                },
                FuzzType::AFLFuzz => {
                    AFLFuzz::new(handler).run();
                },
                FuzzType::LenFuzz => {
                    LenFuzz::new(handler).run();
                },
                FuzzType::CmpFnFuzz => {
                    FnFuzz::new(handler).run();
                },
                FuzzType::OtherFuzz => {
                    warn!("Unknown fuzz type!!");
                },
            }
            unsafe{
                //READ FROM FUNCTION SHMEM
                let mut res: u32 = 0;
                if(!skipShm){
                  let slice = shmem.as_slice();
                  let mut n = [0u8;4];
                  n.clone_from_slice(&slice[0..4]);
                  res = u32::from_le_bytes(n);
                } 
                //FIND DIFFERENCE FUZZ_LOOPS - REACHED_TIMES
                let mut diff: u64 = 0;
                if(res as u64 >= localCounter){
                   diff = 0;
                }else{
                   diff = localCounter - (res as u64);
                }
              
                //READ FROM MAIN SHMEM
                let slice_main = shmem_main.as_slice();
                let mut n_main = [0u8;4];
                n_main.clone_from_slice(&slice_main[0..4]);
                let mut res_main = u32::from_le_bytes(n_main);
 
                if(!skipShm && printIt){
                  println!("{} loops, {} main execs", localCounter, res_main);
                  println!("{} loops, {} function execs", localCounter, res);
                }
                
                //CHECK IF THE VARIABLES ARE TOO BIG
                if((totalNotReached >= u64::MAX-diff) || (totalBinaryExecution >=u64::MAX-localCounter)){
                    println!("!!!CAUTION the variables about to be out of range!"); 
                  if(totalNotReached>=u64::MAX-diff){
                    totalNotReachedBucket+=1;
                    totalNotReached = totalNotReached - (u64::MAX-diff);
                  }else{
                    totalBinaryExecutionBucket+=1;
                    totalBinaryExecution=totalBinaryExecution-(u64::MAX-localCounter);
                  }
                }
                if(!skipShm){
                  //UPDATE RESULT
                  totalNotReached+=diff;
                  totalBinaryExecution+=localCounter;
                } 
            };
        }
       
        depot.update_entry(cond);
    }
    //SOME SLEEPER IDK WHY
    let sleeper=time::Duration::from_millis(10000);
    thread::sleep(sleeper);

    println!("STATISTICS");
    unsafe{println!("{} times functions were not reached",totalNotReached);}
    println!("{} times functions were tried to be reached",totalBinaryExecution);
    unsafe{println!("{}% - is (not reached/tried to be)", ((totalNotReached as f64 )/(totalBinaryExecution as f64)) *100.0);};
    println!("Buckets = ({}, {}) (not required if the number were in range of u64).",totalNotReachedBucket,totalBinaryExecutionBucket); 

}
