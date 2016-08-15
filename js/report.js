WorkerScript.onMessage = function(message) {
    if(message.action=='startreport'){

        WorkerScript.sendMessage({'type':'report','action':'add'});
    }
    else if(message.action=='startpause'){

        WorkerScript.sendMessage({'type':'pause','action':'add'});
    }
    else if(message.action=='endpause'){

    }
    else if(message.action=='endreport'){

    }
    else{
        console.log("Erreur");
    }
}
