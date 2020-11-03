$(document).ready(function(){
    //TODO:

    var socket = io(`http://192.168.1.2:3000`);

    //request data:
    socket.emit(`connection`, "Hello");
    socket.emit(`get-data`, `Get data`);
    socket.emit(`esp_sentdata`, {'name':`ESP`});

    socket.on(`data-homepage`, (data) => {
        $(".form-group").empty();
        var checked='';
        var isOn=1;
        var html;
        data["data"].forEach(port => {
            if(port['status']==1){
                checked = 'checked';
                isOn=0;
            }
            else{
                checked='';
                isOn=1;
            }
            html="<div class ='form-check'><input class='form-check-input' type='checkbox'"+ checked + " onclick='onLed($(this).val(),"+ isOn +")' value='" + port['id']+"'> GPIO "+ port['id'] +"</div>";
            $(".form-group").append(html);
        }); 
    });

    socket.on(`data-change`, data => {
        console.log(data);
    });

    socket.on('status-port-change', data => {
        console.log(data);
    });

});

function onLed(port, status){
    var data={
        port:port,
        status: status
    };
    var success=function(data){

        location.reload();
    };
    var datatype='text';
    $.post('/ajax',data,success,datatype);
}