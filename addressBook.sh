create () {
    echo "Enter address book name"
    read bn
    
    touch "$bn"
    echo "Address book created"
}

display () {
    echo "Enter address book name"
    read bn
    
    res=$(ls | grep -w "$bn" | wc -w)
    
    if [ "$res" -gt 0 ]; then
        cat "$bn"
    else
        echo "Error: address book not found"
    fi
}

insert () {
    echo "Enter address book name"
    read bn
    
    res=$(ls | grep -w "$bn" | wc -w)
    
    if [ "$res" -gt 0 ]; then
        echo "Enter name"
        read name
        
        len=$(grep -w "$name" "$bn" | wc -w)
        if [ "$len" -gt 0 ]; then
            echo "Name already exists"
        else
            echo "Enter number"
            read num
            echo "Enter email"
            read email
            
            record="$name $num $email"
            echo "$record" >> "$bn"
            echo "Record inserted"
        fi
    else
        echo "Error: address book does not exist"
    fi
}

modify () {
    echo "Enter address book name"
    read bn
    
    res=$(ls | grep -w "$bn" | wc -w)
    
    if [ "$res" -gt 0 ]; then
        echo "Enter name to modify"
        read name
        
        len=$(grep -w "$name" "$bn" | wc -w)
        if [ "$len" -gt 0 ]; then
            echo "Enter new details (name number email):"
            read new_name new_num new_email
            
            old_record=$(grep -w "$name" "$bn")
            new_record="$new_name $new_num $new_email"
            
            sed -i "s|$old_record|$new_record|g" "$bn"
            echo "Record updated"
        else
            echo "Name does not exist"
        fi
    else
        echo "Error: address book does not exist"
    fi
}


delete () {
    delete () {
    echo "Enter address book name"
    read bn
    
    res=$(ls | grep -w "$bn" | wc -w)
    
    if [ "$res" -gt 0 ]; then
        echo "Enter the name of the person to delete"
        read name
        
        len=$(grep -w "$name" "$bn" | wc -w)
        if [ "$len" -gt 0 ]; then
            # Delete the line containing the name
            sed -i "/\b$name\b/d" "$bn"
            echo "Record deleted"
        else
            echo "Name does not exist in the address book"
        fi
    else
        echo "Error: address book does not exist"
    fi
}

}    

while :
do
    echo "Enter your choice: "
    echo "1.Create"
    echo "2.Display"
    echo "3.Insert"
    echo "4.Modify"
    echo "5.Delete"
    echo "6.Exit"
    read choice
    
    case $choice in
        1) create ;;
        2) display ;;
        3) insert ;;
        4) modify ;;
        5) delete ;;
        6) exit ;;
        *) echo "Invalid input" ;;
    esac
done

