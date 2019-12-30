#
# Looks at the last X number of Security Event Logs, specifically at a
# property (ReplacementStrings) that contains an array of strings
# specific to the event. The strings usually contain one or more SIDs,
# of which local account the script tries to look up the name of.
#
# There's no purpose to this other than that I was bored and thought
# up a quick one-liner while reading Chapter 4. I'm sure sure there's
# also already a better cmdlet out there for converting SIDs to
# principals
#
# Also, this script needs to run with administrative privileges,
# otherwise access to the Security Log is denied
#

# The original one-liner
# (0..9) | % { Write-Host Entry $_`:; $sids = (get-eventlog -logname Security -Newest 10)[$_].ReplacementStrings | ? { $_ -like "S-*" }; $sids | % { Write-Host `t$_ `(TBD`)} } 

function Get-SecurityEventSids {
	 param([int]$num_events = 10)

	 # Get the last X events
	 $events = Get-EventLog -logname Security -Newest $num_events

	 # Go over every event to collect SIDs and Names from them 
	 $counter = 0
	 # todo(dallas): what to do if/when there's no SID in the array
	 $events | % {
		 Write-Host "Event $counter`:"; $counter++

		 $sids = [System.Collections.ArrayList]@()
		 $names = [System.Collections.ArrayList]@()

       	 	 # Build the list of SIDS from ReplacementStrings
		 $strings = $_.ReplacementStrings
		 $strings | ? {$_ -Like "S-*" } | % { $sids.Add($_) } > $null

		 # Build the list of names from the collected SIDs
		 $sids | % {
		       # todo(dallas): better error handling
		       # 	       this is just gross
		       $res = Get-LocalUser -SID $_ 2> $null
		       if($res -eq $null) { # user not found, try group
		       	       $res = Get-LocalGroup -SID $_ 2> $null
		       }
		       # todo(dallas): what to do if no SID is found
	 	       # 	       (eg. for accounts like SYSTEM)
		       if($res -eq $null) { $res = "!! NOT FOUND !!" } # oh well we tried
		       $names.Add($res) > $null
		 }

		 for($i = 0; $i -lt $sids.Count; $i++) {
       	 	 	Write-Host "`t$($sids[$i]) `($($names[$i])`)"
		 }
         }
}

Get-SecurityEventSids