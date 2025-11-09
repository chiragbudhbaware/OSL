#!/bin/bash
# Simple Address Book (shell script)
# Fields: Name|Phone|Email|Address
# Each line is one record. Records are numbered for delete/modify operations.

FILE="address_book.txt"
TMP="address_book.tmp"

# ensure file exists
touch "$FILE"

pause() {
  echo
  read -rp "Press Enter to continue..."
}

show_header() {
  printf "\n%-4s %-20s %-15s %-25s %s\n" "No." "Name" "Phone" "Email" "Address"
  printf "%s\n" "--------------------------------------------------------------------------------"
}

create_address_book() {
  if [ -s "$FILE" ]; then
    read -rp "Address book exists and will be overwritten. Continue? (y/n): " ans
    case "$ans" in
      y|Y) > "$FILE"; echo "Address book created (existing data cleared).";;
      *) echo "Create cancelled.";;
    esac
  else
    > "$FILE"
    echo "Empty address book created."
  fi
  pause
}

view_address_book() {
  if [ ! -s "$FILE" ]; then
    echo "Address book is empty."
    pause
    return
  fi

  show_header
  nl -w3 -s'  ' "$FILE" | while IFS= read -r line; do
    # line format: <num>  Name|Phone|Email|Address
    num="${line%%  *}"            # number (nl output)
    rest="${line#*  }"            # rest: Name|Phone|Email|Address
    IFS='|' read -r name phone email addr <<< "$rest"
    printf "%-4s %-20s %-15s %-25s %s\n" "$num" "$name" "$phone" "$email" "$addr"
  done
  pause
}

insert_record() {
  echo "Enter the details to insert:"
  read -rp "Name    : " name
  read -rp "Phone   : " phone
  read -rp "Email   : " email
  read -rp "Address : " addr

  # replace '|' inside fields to avoid breaking format
  name="${name//|/-}"
  phone="${phone//|/-}"
  email="${email//|/-}"
  addr="${addr//|/-}"

  echo "${name}|${phone}|${email}|${addr}" >> "$FILE"
  echo "Record inserted."
  pause
}

delete_record() {
  if [ ! -s "$FILE" ]; then
    echo "Address book is empty."
    pause
    return
  fi

  view_address_book
  read -rp "Enter record number to DELETE: " num
  if ! [[ $num =~ ^[0-9]+$ ]]; then
    echo "Invalid number."
    pause
    return
  fi

  total=$(wc -l < "$FILE")
  if [ "$num" -lt 1 ] || [ "$num" -gt "$total" ]; then
    echo "Record number out of range (1..$total)."
    pause
    return
  fi

  # delete that line
  sed "${num}d" "$FILE" > "$TMP" && mv "$TMP" "$FILE"
  echo "Record $num deleted."
  pause
}

modify_record() {
  if [ ! -s "$FILE" ]; then
    echo "Address book is empty."
    pause
    return
  fi

  view_address_book
  read -rp "Enter record number to MODIFY: " num
  if ! [[ $num =~ ^[0-9]+$ ]]; then
    echo "Invalid number."
    pause
    return
  fi

  total=$(wc -l < "$FILE")
  if [ "$num" -lt 1 ] || [ "$num" -gt "$total" ]; then
    echo "Record number out of range (1..$total)."
    pause
    return
  fi

  # show current values
  line=$(sed -n "${num}p" "$FILE")
  IFS='|' read -r cur_name cur_phone cur_email cur_addr <<< "$line"

  echo "Leave field empty to keep current value."
  read -rp "Name    [$cur_name] : " name
  read -rp "Phone   [$cur_phone]: " phone
  read -rp "Email   [$cur_email]: " email
  read -rp "Address [$cur_addr] : " addr

  # if empty, use current
  name=${name:-$cur_name}
  phone=${phone:-$cur_phone}
  email=${email:-$cur_email}
  addr=${addr:-$cur_addr}

  # sanitize
  name="${name//|/-}"
  phone="${phone//|/-}"
  email="${email//|/-}"
  addr="${addr//|/-}"

  newrec="${name}|${phone}|${email}|${addr}"

  # replace line
  awk -v n="$num" -v rec="$newrec" 'NR==n { print rec; next } { print }' "$FILE" > "$TMP" && mv "$TMP" "$FILE"
  echo "Record $num modified."
  pause
}

menu() {
  clear
  echo "========== Simple Address Book =========="
  echo "a) Create address book"
  echo "b) View address book"
  echo "c) Insert a record"
  echo "d) Delete a record"
  echo "e) Modify a record"
  echo "f) Exit"
  echo "========================================="
}

while true; do
  menu
  read -rp "Choose an option (a-f): " opt
  case "$opt" in
    a|A) create_address_book ;;
    b|B) view_address_book ;;
    c|C) insert_record ;;
    d|D) delete_record ;;
    e|E) modify_record ;;
    f|F) echo "Goodbye."; exit 0 ;;
    *) echo "Invalid choice."; pause ;;
  esac
done
