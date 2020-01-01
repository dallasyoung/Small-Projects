$creds = (get-credential)
Write-Host Your password is: $($creds.GetNetworkCredential().password)