/* 
------------------
MAIN JS FILE
-----------------
*/

/* Object data with two arrays, one for todo tasks, second for archived tasks. If we have locally stored data, we have to load them.*/
var data = (localStorage.getItem('todoList')) ? JSON.parse(localStorage.getItem('todoList')) :{
	todo:[],
	archived:[]
};

/* Function for clearing localStorage data. If everything works, it's commented.*/
//localStorage.clear();

console.log(data);

/* Calling function for rendering todo tasks which are locally stored.*/
renderTodoList();

input_forms = [ 'input-form-ifj', 'input-form-ial', 'input-form-itu',
				'input-form-inm', 'input-form-inp', 'input-form-ips',
				'input-form-izp', 'input-form-isu']

/* Global variables, we use them to add new todo task.*/
var subject_object;
var subject_name;
var subject_color;
var subject_activity;
var subject_info;

/* Function for displaying todo tasks. If it has extra parameter filter, we are filtering tasks by subject. If it's null we display all todo tasks.*/
function renderTodoList(filter)
{
	if(!data.todo.length)
		return;

	for(var i = 0; i < data.todo.length;i++)
	{
		var temp = JSON.parse(data.todo[i]);
		var name=temp.name;
		var color=temp.color;
		var activity=temp.activity;
		var info=temp.info;

		if((filter==null) || (filter===JSON.stringify(name)))
			addItemHTML(name, color, activity, info);
	}
}

/* Function for displaying archived tasks. Very similar to renderTodoList but we are working with different array,
 *  we don't filter tasks in archive and addItemHTML has extra parameter, which tell us that is archived tasks we want to display*/
function renderArchiveList()
{
	if(!data.archived.length)
		return;

	for(var i = 0; i < data.archived.length;i++)
	{
		var temp = JSON.parse(data.archived[i]);
		var name=temp.name;
		var color=temp.color;
		var activity=temp.activity;
		var info=temp.info;
		
		addItemHTML(name, color, activity, info, 1);

	}
}

/* Function for choosing and highlighting subject. We use global variables to store important data.*/
function choose_subject_to_add(object_id)
{
	/* Firstly, reset brightness to default values */
	for (var i = 0; i < input_forms.length; i++)
	{
		document.getElementById(input_forms[i]).style["-webkit-filter"] = "brightness(100%)";
	}

	/* Then highlight the selected input form cell */
	var object = document.getElementById(object_id)
	object.style["-webkit-filter"] = "brightness(150%)";

	subject_object = object;
	subject_name=subject_object.textContent;
	subject_color=subject_object.getAttribute('bgcolor');
}

/* Functions for storing infos of task to global variable.*/
function set_activity_info()
{
	subject_activity = document.getElementById("activity_info").value;
}
function set_further_info()
{
	subject_info = document.getElementById("further_info").value
}

/* This function is called whenever we change data to immediatly store data to local storage.*/
function dataObjectUptated()
{
	localStorage.setItem('todoList', JSON.stringify(data));
}

/* Function for converting record of RGB color to HEX record of color.*/
function rgb2hex(rgb) 
{
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
    function hex(x) {
        return ("0" + parseInt(x).toString(16)).slice(-2);
    }
    return "#" + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
}

/* Function for sending todo task to archive. We need to remove it from todo array and push it to archived array and undisplay it from screen.*/
function itemToArchive()
{
	var item = this.parentNode;
	var parent = item.parentNode;
	
	var name = item.children[0].textContent;
	var color = rgb2hex($(item).css("border-left-color"));
	var activity= item.children[1].textContent;
	
	if( $(item.children[2]).css("display") == "none")
	{
		$(item).find(".extra-info").css("display","block");
		var info=item.children[2].textContent;
		$(item).find(".extra-info").css("display","none");
	}
	else
	{
		var info=item.children[2].textContent;
	}
		
	var temp = {'name': name,'color': color,'activity': activity, 'info': info};

	data.todo.splice(data.todo.indexOf(JSON.stringify(temp)),1);
	data.archived.push(JSON.stringify(temp));
	dataObjectUptated();
	console.log(data);
	
	$('.popup span').text('Task send to archive');
	$('.popup').show().delay(2000).fadeOut();
	
	parent.removeChild(item);
}

/* Function for deleting task. We need also to undisplay it from screen.*/
function itemDelete()
{
	var item = this.parentNode;
	var parent = item.parentNode;	
	
	var name = item.children[0].textContent;
	var color = rgb2hex($(item).css("border-left-color"));
	var activity= item.children[1].textContent;
	
	if( $(item.children[2]).css("display") == "none")
	{
		$(item).find(".extra-info").css("display","block");
		var info=item.children[2].textContent;
		$(item).find(".extra-info").css("display","none");
	}
	else
	{
		var info=item.children[2].textContent;
	}
	
	var temp = {'name': name,'color': color,'activity': activity, 'info': info};
	data.archived.splice(data.archived.indexOf(JSON.stringify(temp)),1);
	data.todo.splice(data.todo.indexOf(JSON.stringify(temp)),1);

	dataObjectUptated();
	console.log(data);
	
	$('.popup span').text('Task deleted');
	$('.popup').show().delay(2000).fadeOut();
	
	parent.removeChild(item);
}

/* Function for sending archived task back to todo list tasks.*/
function fromArchiveToTODO()
{
	var item = this.parentNode;
	var parent = item.parentNode;	
	
	var name = item.children[0].textContent;
	var color = rgb2hex(item.style.borderColor);
	var activity= item.children[1].textContent;
	
	if( $(item.children[2]).css("display") == "none")
	{
		$(item).find(".extra-info").css("display","block");
		var info=item.children[2].textContent;
		$(item).find(".extra-info").css("display","none");
	}
	else
	{
		var info=item.children[2].textContent;
	}
	var temp = {'name': name,'color': color,'activity': activity, 'info': info};
	data.archived.splice(data.archived.indexOf(JSON.stringify(temp)),1);
	data.todo.push(JSON.stringify(temp));
	dataObjectUptated();
	console.log(data);
	
	$('.popup span').text('Task send to TODO list');
	$('.popup').show().delay(2000).fadeOut();
	
	parent.removeChild(item);
	
}

/* Function for adding HTML code of tasks. If we us extra parameter archived then we are generating code for archive task,
 *  which is little bit different then todo task.*/
function addItemHTML(name, color, activity, info, archived)
{
	/* Get subject list element */
	var ul = document.getElementById("subject_list");
		
	/* Create list item */
	var li = document.createElement("li");
	li.setAttribute('class','list-item');
	li.style.borderColor = color;
	ul.insertBefore(li, ul.childNodes[0]);
		
	/* Create subject header */
	var header = document.createElement("header");
	header.setAttribute('class','item-header');
	header.appendChild(document.createTextNode(name));
	li.appendChild(header);
			
	/* Set subject activity text */
	var div = document.createElement("div");
	div.setAttribute('class','item-info');
	div.appendChild(document.createTextNode(activity));
	li.appendChild(div);
		
	/* Set subject extra info text */
	var div2 = document.createElement("div");
	div2.setAttribute('class','extra-info');
	div2.style.display = "none";
	div2.appendChild(document.createTextNode(info));
	li.appendChild(div2);
	
	/* Create the 'Task complete' button */
	var button = document.createElement("button");
	button.setAttribute('class','item-button');
	li.appendChild(button);
		
	var i = document.createElement("i");
	
	/* If it is archived tasks that we are rendering, then it will have different class and different button.*/
	if (archived)
	{
		i.setAttribute('class', "fa fa-fw fa-arrow-alt-circle-up");
		button.addEventListener('click', fromArchiveToTODO);
		li.setAttribute('class', 'list-item archived');
	}
	else
	{
		i.setAttribute('class', "fa fa-fw fa-check");
		button.addEventListener('click', itemToArchive);
	}

	button.appendChild(i);	

	/* Creating remove button, which is hidden by default.*/
	var remove_btn = document.createElement("button");
	remove_btn.setAttribute('class','remove-button');
	var j = document.createElement("i");
	j.setAttribute('class', "fa fa-fw fa-times-circle");
	remove_btn.appendChild(j);
	li.appendChild(remove_btn);
	remove_btn.addEventListener('click', itemDelete);
	remove_btn.style.display = "none";

	
}

/* Function for adding tasks by completing form. You have to enter all values and then click on button: Add new task,
 * values are reseted after clicking on button.*/
function add_new_task()
{
	$('#subject_label').css("color", 'white');
	$('#activity_label').css("color", 'white');
	$('#info_label').css("color", 'white');
	
	if(!subject_name)
		$('#subject_label').css("color", 'red');
	if(!subject_activity)
		$('#activity_label').css("color", 'red');
	if(!subject_info)
		$('#info_label').css("color", 'red');
	
	/* All three variables are not empty */
	if (subject_object && subject_name && subject_color && subject_activity && subject_info)
	{
		display_all_todo_tasks();
		
		addItemHTML(subject_name, subject_color, subject_activity, subject_info);
		
		subject_object.style["-webkit-filter"] = "brightness(100%)";
		document.getElementById("activity_info").value='';
		document.getElementById("further_info").value='';

		var temp={'name': subject_name,'color': subject_color,'activity': subject_activity, 'info': subject_info};
		data.todo.push(JSON.stringify(temp));
		dataObjectUptated();
		console.log(data);
		
		/*This two lines of code are used to display prompts on screen.*/
		$('.popup span').text('New task added');
		$('.popup').show().delay(2000).fadeOut();
		
		subject_object=null;
		subject_name=null;
		subject_color=null;
		subject_activity=null;
		subject_info=null;
	}
	
	else
	{
		$('.popup span').text('Something is missing.');
		$('.popup').show().delay(2000).fadeOut();
	}
}

/* This function is called whenever we want filter tasks by subjects.*/
$('#subjects_filtering .category').on('click', function() {

	$(".category").each( function() {
		$(this).removeClass("selected");
	});

	$(this).addClass("selected");
	
	var name = (JSON.stringify(this.textContent));
	var parent =document.getElementById("subject_list");
  
	name = name.replace(/\s/g,'');
	name = name.replace(/\\n/g,'');

	$('#subject_list li').remove();
	
	/* If it is selected ALL then we render all todo tasks, when it's selected subject we are calling the same function but with extra parameter*/
	(! name.localeCompare(JSON.stringify('ALL'))) ? renderTodoList() : renderTodoList(name)
});

/* Function for displaying archive.*/
$('#archive').click(function() {
	
	hide_form();
	$(".site-menu").hide();
	$(".site-main").css("grid-column", "span 6");

	$('#subject_list li').remove();
  
	renderArchiveList();
});

/* Function for displaying all todo tasks.*/
function display_all_todo_tasks()
{
	$('#subject_list li').remove();
	
	renderTodoList();
}

/* Function called whenever we click on task. Cell of tasks will be larger and there will be displayed extra info a remove button.*/
$('#subject_list').on('click', '.list-item', function() {
	
	if( $(this).css("grid-column-end") == "span 2")
	{
		$(this).css( "grid-column-end", "span 1" );
		$(this).find(".extra-info").css("display","none");
		
		if($(this).find(".remove-button").css("display") == "block")
			$(this).find(".remove-button").css("display","none");
		
	}
	else
	{
		$(this).css( "grid-column-end", "span 2" );
		$(this).find(".extra-info").css("display","block");
		$(this).find(".remove-button").css("display","block");
	}
});

function logout()
{
	var confirmation = confirm("Are you sure you want to logout ?");
	if (confirmation == true)
	{
		/* 'Removes' all element from the page */
		var site_body = document.getElementById('site');
		while (site_body.firstChild)
		{
			site_body.removeChild(site_body.firstChild);
		}
		
		/* Set temporarily new background colour */
		document.body.style.background = "transparent";
		document.body.style.backgroundColor = '#3a7bd5';
		
		/* The login button */
		var login_button = document.createElement("button");
		var login_text = document.createTextNode("Login");
		login_button.onclick = function() { window.location.reload(); }
		login_button.appendChild(login_text);   
		
		/* Logout text */
		var logout_text = document.createElement("div");
		logout_text.setAttribute('class', 'logout');
		logout_text.innerHTML = "You have been successfully logged out"
		
		/* Make a new line */
		var new_line = document.createElement("br");
		logout_text.appendChild(new_line);
		
		/* Add login button */
		logout_text.appendChild(login_button);
		
		/* Add all to html body*/
		document.body.appendChild(logout_text);
		
		return true;
	}
	
	else
		return false;
} 


function show_form()
{
	var form = document.getElementById("form");
	form.setAttribute('class', 'sidebar-form visible');
}

function hide_form() {
	var form = document.getElementById("form");
	form.setAttribute('class', 'sidebar-form');
}

/* Displaying all todo tasks, when someone has clicked on button todo-list*/
$('#todo-list').click(function() {
	
	hide_form();
	$(".site-menu").show();
	$(".site-main").css("grid-column", "span 4");

    $('#subject_list li').remove();
	renderTodoList();

	$(".category").each(function () {
		$(this).removeClass("selected");
	});

	$("#all").addClass("selected");
	
});
