Write-Host "This script deploys the TroubleshootingTest Module which enables testing
of Windows Troubleshooting Pack scripts in standard PowerShell contexts.
This will enable you to invoke and to view help about the following cmdlets:
  * Update-DiagRootCause
  * Get-DiagInput
  * Write-DiagProgress
  * Update-DiagReport"


#Get information about the current system
$OSInfo = gwmi Win32_OperatingSystem

#Check OS Version   
$VersionArray = $OsInfo.version.split(".")
if (($versionArray[0] -ge 6) -and ($VersionArray[1] -ge 1))
{
    Write-Host "Windows 7 or Newer Machine Detected."
}
else
{
    Write-Error "This version of the setup requires Windows 7. Setup Aborted."
    return
}

#Check if script Execution Is enabled
$ExecutionPolicy = Get-ExecutionPolicy


if (($ExecutionPolicy -ieq "AllSigned") -or ($ExecutionPolicy -ieq "Restricted"))
{
Write-Error -Exception "The current PowerShell execution policy prevents execution of unsigned scripts
In order to proceed, you must enable execution of unsigned PowerShell scripts on this system.
Please set the execution policy to RemoteSigned or Unrestricted before attempting to run setup again.
For more information, type: 'get-help Set-ExecutionPolicy'"
return
}


#Check if already installed     
$profiledir = Split-Path $profile
          
if ( -not (Test-Path $profiledir) ) 
{
    $result = mkdir $profiledir
}

$moduledir = $profiledir + "\Modules"

if ( -not (Test-Path $moduledir) ) 
{
    $result = mkdir $moduledir
}

$modulesource = (Split-Path $MyInvocation.MyCommand.Path) + "\PowerShellModule\troubleshootingtest"   

$installed = Test-Path ($moduledir + "\troubleshootingtest")


if ($installed)
{

    Write-Host "TroubleshootingTest module is already installed on this system. Exiting installer."
    return
}
        
#deploy powershell module
Write-Host "Copying Test Mode module to target location..."
Copy-Item $modulesource -Destination $moduledir -Recurse 
     
        
#Detect if we need to add the module declarations to the PowerShell profile
$addtoprofile = $true
$baseprofile = $profiledir + "\profile.ps1"

if (test-path $baseprofile )
{
    if (Get-Content $baseprofile | where {$_.indexof("#Windows Troubleshooting Test Mode Cmdlets") -ne -1})
    { 
        $addtoprofile = $false
    }
}
                      


if ($addtoprofile) 
{
    Add-Content $baseprofile "
#Windows Troubleshooting Test Mode Cmdlets
Import-Module troubleshootingtest
"
    Write-Host "Added TroubleshootingTest Module to base PowerShell Profile (profile.ps1)"         
}
                             
Write-Host "TroubleshootingTest Module deployed."
Import-Module troubleshootingtest


# SIG # Begin signature block
# MIIa+wYJKoZIhvcNAQcCoIIa7DCCGugCAQExCzAJBgUrDgMCGgUAMGkGCisGAQQB
# gjcCAQSgWzBZMDQGCisGAQQBgjcCAR4wJgIDAQAABBAfzDtgWUsITrck0sYpfvNR
# AgEAAgEAAgEAAgEAAgEAMCEwCQYFKw4DAhoFAAQUvZAgnxyvOadYQ52BHBrPXLUW
# P16gghXYMIIEhTCCA22gAwIBAgIKYQX3HgAAAAAAMjANBgkqhkiG9w0BAQUFADB5
# MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3RvbjEQMA4GA1UEBxMHUmVk
# bW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9uMSMwIQYDVQQDExpN
# aWNyb3NvZnQgQ29kZSBTaWduaW5nIFBDQTAeFw0wOTA3MTMyMzAwMThaFw0xMDEw
# MTMyMzEwMThaMIGDMQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3RvbjEQ
# MA4GA1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9u
# MQ0wCwYDVQQLEwRNT1BSMR4wHAYDVQQDExVNaWNyb3NvZnQgQ29ycG9yYXRpb24w
# ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC1jKmclIRhGswuIsSgk2QJ
# 3oWhsJBd/MlsaLXx7/yOa/T4oG7XUd5goydj1PbftkzSRQunPdhv/u2nGGPTH8aB
# 1L8HN5dAK3/nX1o12ZNPq/MqkvCmjYHsBSTc27hhY1NhAjU41rj17ntUpslYNen0
# Fe/aB2wFLU2ktrqbO7gxMH5FvKmhJTkMQ6RHO5ecIERyN89QoE1N9MNl3ETRCDs4
# 3T7FF2xGx61zJMCPicHq6rrNaKkS1lgMUxYM/jkD1oHyPlxX83rbZuEHGNkZAxT2
# 5kWZKlFMkNda76+3OwwpTW0grge3ki7oacqfMELnwzKKCyJKMuicDMZRAyYBbZJz
# AgMBAAGjggECMIH/MBMGA1UdJQQMMAoGCCsGAQUFBwMDMB0GA1UdDgQWBBSHgbff
# 7vp3+lkFYzcECtfp2qDg4DAOBgNVHQ8BAf8EBAMCB4AwHwYDVR0jBBgwFoAUV0V0
# HF2w9shDBeCMVC2PMqf+SJYwSQYDVR0fBEIwQDA+oDygOoY4aHR0cDovL2NybC5t
# aWNyb3NvZnQuY29tL3BraS9jcmwvcHJvZHVjdHMvQ29kZVNpZ1BDQS5jcmwwTQYI
# KwYBBQUHAQEEQTA/MD0GCCsGAQUFBzAChjFodHRwOi8vd3d3Lm1pY3Jvc29mdC5j
# b20vcGtpL2NlcnRzL0NvZGVTaWdQQ0EuY3J0MA0GCSqGSIb3DQEBBQUAA4IBAQA2
# IYT1j4MhTYeoP4vnxaQPTDBuWXSbC5d0Fqmx83Ckr8IMMyTzoEr6weQhxrbZ6lxb
# kWH3Im3/MfwXTtfNQOOd0uW00vqNbAfad5rKK+4NWUfPBMIQZ/rq6UM7t+bRnNmO
# I2BuEpvD2X5cVzLdlA3ffcVIkk1IdreiwFOVPvw6o5N88oUEx8aFOJc2+BKTIgCZ
# TnmvlutaIIOs0FxaMLAA4ms5jySJ2j0MpEZ5YjZTatfJzZII+4flVPV8WcXEd32w
# wSyYx5dsQ6f4IPiDVGSeYPjiSqVn1Em/VIh9UkTzUcAm0cV88InWEQkowkVFgB2Q
# Ir2OogU+PLNPvdc54IiTMIIEuzCCA6OgAwIBAgIKYQTKaQAAAAAACDANBgkqhkiG
# 9w0BAQUFADB3MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3RvbjEQMA4G
# A1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9uMSEw
# HwYDVQQDExhNaWNyb3NvZnQgVGltZS1TdGFtcCBQQ0EwHhcNMDcwNjA1MjIwMzIx
# WhcNMTIwNjA1MjIxMzIxWjCBpDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCldhc2hp
# bmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jw
# b3JhdGlvbjEnMCUGA1UECxMebkNpcGhlciBEU0UgRVNOOkE1QjAtQ0RFMC1EQzk0
# MSUwIwYDVQQDExxNaWNyb3NvZnQgVGltZS1TdGFtcCBTZXJ2aWNlMIIBIjANBgkq
# hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAr/ha9jSxog+OrM/8GPCtJQ1xu19btzs3
# QRMkHOojxa6smcDcpiPwyqkzMDJqL5ohQgIJJlmbUOlXcYtn65foBHOLeLGYQEaj
# WgEoABag2sVOi6kJVJExnP/EkJVFBx2AKAUGcYDLrDhnYezCXxadf7A/0Sr3Mm9z
# PXxumDaahu4AOO4X7E9VM0aBekDcpfXSI4ZdKzNPJEiTGrG0fs/fMxu/1VvGLxPz
# 5oS3TpOYvADrXB1pxcjI0sQ8sAAU47Kc+aEeCdzV9W18MjEzso9Zm/7KH10iTroS
# HfBnwwSJS38D/X3XVIEDXUn1HqsgftJltHEZ6Ex9xpS9hZFtUFrDqQIDAQABo4IB
# GTCCARUwHQYDVR0OBBYEFJ6PqHTBQu4YzT296dbuJ7DHNTRnMB8GA1UdIwQYMBaA
# FCM0+NlSRnAK7UD7dvuzK7DDNbMPMFQGA1UdHwRNMEswSaBHoEWGQ2h0dHA6Ly9j
# cmwubWljcm9zb2Z0LmNvbS9wa2kvY3JsL3Byb2R1Y3RzL01pY3Jvc29mdFRpbWVT
# dGFtcFBDQS5jcmwwWAYIKwYBBQUHAQEETDBKMEgGCCsGAQUFBzAChjxodHRwOi8v
# d3d3Lm1pY3Jvc29mdC5jb20vcGtpL2NlcnRzL01pY3Jvc29mdFRpbWVTdGFtcFBD
# QS5jcnQwEwYDVR0lBAwwCgYIKwYBBQUHAwgwDgYDVR0PAQH/BAQDAgbAMA0GCSqG
# SIb3DQEBBQUAA4IBAQBnpd8oUdxIg/3GZfld+maqso2nVp2iSL4yrG2XD08MRLu7
# GPSQ5+J9xXQA1obzkcDQ0QWJXmKL+mj3Zc/mU6nfdBIhrYwVtKbKQ5PJa+FKsu0P
# zrSKOn/3tcy8Epp7zwTEP9kKLHU8kp7PBjZiDgIX5pkn9/tx7/9aemFVurqYh+tx
# rHb16IafNmdQQXj8M6TVaXMByHAja3yjV8hoB+RbQcH4Jv1pSQOZ1q/JP6T1nEz5
# Cp3SzUu/ffoWkOw7cUn+BBXK4mraaQu0kz40/srsgEnksnQYzJHnPk63e/VAj2Rb
# 1FDVOU1Tzb7UXkeCW9FaepkuRnBrB4UNbAkTYw91MIIGBzCCA++gAwIBAgIKYRZo
# NAAAAAAAHDANBgkqhkiG9w0BAQUFADBfMRMwEQYKCZImiZPyLGQBGRYDY29tMRkw
# FwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0MS0wKwYDVQQDEyRNaWNyb3NvZnQgUm9v
# dCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwHhcNMDcwNDAzMTI1MzA5WhcNMjEwNDAz
# MTMwMzA5WjB3MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3RvbjEQMA4G
# A1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0aW9uMSEw
# HwYDVQQDExhNaWNyb3NvZnQgVGltZS1TdGFtcCBQQ0EwggEiMA0GCSqGSIb3DQEB
# AQUAA4IBDwAwggEKAoIBAQCfoWyx39tIkip8ay4Z4b3i48WZUSNQrc7dGE4kD+7R
# p9FMrXQwIBHrB9VUlRVJlBtCkq6YXDAm2gBr6Hu97IkHD/cOBJjwicwfyzMkh53y
# 9GccLPx754gd6udOo6HBI1PKjfpFzwnQXq/QsEIEovmmbJNn1yjcRlOwhtDlKEYu
# J6yGT1VSDOQDLPtqkJAwbofzWTCd+n7Wl7PoIZd++NIT8wi3U21StEWQn0gASkdm
# EScpZqiX5NMGgUqi+YSnEUcUCYKfhO1VeP4Bmh1QCIUAEDBG7bfeI0a7xC1Un68e
# eEExd8yb3zuDk6FhArUdDbH895uyAc4iS1T/+QXDwiALAgMBAAGjggGrMIIBpzAP
# BgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBQjNPjZUkZwCu1A+3b7syuwwzWzDzAL
# BgNVHQ8EBAMCAYYwEAYJKwYBBAGCNxUBBAMCAQAwgZgGA1UdIwSBkDCBjYAUDqyC
# YEBWJ5flJRP8KuEKU5VZ5KShY6RhMF8xEzARBgoJkiaJk/IsZAEZFgNjb20xGTAX
# BgoJkiaJk/IsZAEZFgltaWNyb3NvZnQxLTArBgNVBAMTJE1pY3Jvc29mdCBSb290
# IENlcnRpZmljYXRlIEF1dGhvcml0eYIQea0WoUqgpa1Mc1j0BxMuZTBQBgNVHR8E
# STBHMEWgQ6BBhj9odHRwOi8vY3JsLm1pY3Jvc29mdC5jb20vcGtpL2NybC9wcm9k
# dWN0cy9taWNyb3NvZnRyb290Y2VydC5jcmwwVAYIKwYBBQUHAQEESDBGMEQGCCsG
# AQUFBzAChjhodHRwOi8vd3d3Lm1pY3Jvc29mdC5jb20vcGtpL2NlcnRzL01pY3Jv
# c29mdFJvb3RDZXJ0LmNydDATBgNVHSUEDDAKBggrBgEFBQcDCDANBgkqhkiG9w0B
# AQUFAAOCAgEAEJeKw1wDRDbd6bStd9vOeVFNAbEudHFbbQwTq86+e4+4LtQSooxt
# YrhXAstOIBNQmd16QOJXu69YmhzhHQGGrLt48ovQ7DsB7uK+jwoFyI1I4vBTFd1P
# q5Lk541q1YDB5pTyBi+FA+mRKiQicPv2/OR4mS4N9wficLwYTp2OawpylbihOZxn
# LcVRDupiXD8WmIsgP+IHGjL5zDFKdjE9K3ILyOpwPf+FChPfwgphjvDXuBfrTot/
# xTUrXqO/67x9C0J71FNyIe4wyrt4ZVxbARcKFA7S2hSY9Ty5ZlizLS/n+YWGzFFW
# 6J1wlGysOUzU9nm/qhh6YinvopspNAZ3GmLJPR5tH4LwC8csu89Ds+X57H2146So
# dDW4TsVxIxImdgs8UoxxWkZDFLyzs7BNZ8ifQv+AeSGAnhUwZuhCEl4ayJ4iIdBD
# 6Svpu/RIzCzU2DKATCYqSCRfWupW76bemZ3KOm+9gSd0BhHudiG/m4LBJ1S2sWo9
# iaF2YbRuoROmv6pH8BJv/YoybLL+31HIjCPJZr2dHYcSZAI9La9Zj7jkIeW1sMpj
# tHhUBdRBLlCslLCleKuzoJZ1GtmShxN1Ii8yqAhuoFuMJb+g74TKIdbrHk/Jmu5J
# 4PcBZW+JC33Iacjmbuqnl84xKf8OxVtc2E0bodj6L54/LlUWa8kTo/0wggaBMIIE
# aaADAgECAgphFQgnAAAAAAAMMA0GCSqGSIb3DQEBBQUAMF8xEzARBgoJkiaJk/Is
# ZAEZFgNjb20xGTAXBgoJkiaJk/IsZAEZFgltaWNyb3NvZnQxLTArBgNVBAMTJE1p
# Y3Jvc29mdCBSb290IENlcnRpZmljYXRlIEF1dGhvcml0eTAeFw0wNjAxMjUyMzIy
# MzJaFw0xNzAxMjUyMzMyMzJaMHkxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNo
# aW5ndG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29y
# cG9yYXRpb24xIzAhBgNVBAMTGk1pY3Jvc29mdCBDb2RlIFNpZ25pbmcgUENBMIIB
# IjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAn43fhTeMsQZWZjZO1ArrNiOR
# Hq+rjVjpxM/BnzoKJMTExF6w7hUUxfo+mTNrGWly9HwFX+WZJUTXNRmKkNwojpAM
# 79WQYa3e3BhwLYPJb6+FLPjdubkw/XF4HIP9yKm5gmcNerjBCcK8FpdXPxyY02nX
# MJCQkI0wH9gm1J57iNniCe2XSUXrBFKBdXu4tSK4Lla718+pTjwKg6KoOsWttgEO
# as8itCMfbNUn57d+wbTVMq15JRxChuKdhfRX2htZLy0mkinFs9eFo55gWpTme5x7
# XoI0S23/1O4n0KLc0ZAMzn0OFXyIrDTHwGyYhErJRHloKN8igw24iixIYeL+EQID
# AQABo4ICIzCCAh8wEAYJKwYBBAGCNxUBBAMCAQAwHQYDVR0OBBYEFFdFdBxdsPbI
# QwXgjFQtjzKn/kiWMAsGA1UdDwQEAwIBxjAPBgNVHRMBAf8EBTADAQH/MIGYBgNV
# HSMEgZAwgY2AFA6sgmBAVieX5SUT/CrhClOVWeSkoWOkYTBfMRMwEQYKCZImiZPy
# LGQBGRYDY29tMRkwFwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0MS0wKwYDVQQDEyRN
# aWNyb3NvZnQgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHmCEHmtFqFKoKWtTHNY
# 9AcTLmUwUAYDVR0fBEkwRzBFoEOgQYY/aHR0cDovL2NybC5taWNyb3NvZnQuY29t
# L3BraS9jcmwvcHJvZHVjdHMvbWljcm9zb2Z0cm9vdGNlcnQuY3JsMFQGCCsGAQUF
# BwEBBEgwRjBEBggrBgEFBQcwAoY4aHR0cDovL3d3dy5taWNyb3NvZnQuY29tL3Br
# aS9jZXJ0cy9NaWNyb3NvZnRSb290Q2VydC5jcnQwdgYDVR0gBG8wbTBrBgkrBgEE
# AYI3FS8wXjBcBggrBgEFBQcCAjBQHk4AQwBvAHAAeQByAGkAZwBoAHQAIACpACAA
# MgAwADAANgAgAE0AaQBjAHIAbwBzAG8AZgB0ACAAQwBvAHIAcABvAHIAYQB0AGkA
# bwBuAC4wEwYDVR0lBAwwCgYIKwYBBQUHAwMwDQYJKoZIhvcNAQEFBQADggIBADC8
# sCCkYqCn7zkmYT3crMaZ0IbELvWDMmVeIj6b1ob46LafyovWO3ULoZE+TN1kdIxJ
# 8oiMGGds/hVmRrg6RkKXyJE31CSx56zT6kEUg3fTyU8FX6MUUr+WpC8+VlsQdc5T
# w84FVGm0ZckkpQ/hJbgauU3lArlQHk+zmAwdlQLuIlmtIssFdAsERXsEWeDYD7Pr
# TPhg3cJ4ntG6n2v38+5+RBFA0r26m0sWCG6kvlXkpjgSo0j0HFV6iiDRff6R25SP
# L8J7a6ZkhU+j5Sw0KV0Lv/XHOC/EIMRWMfZpzoX4CpHs0NauujgFDOtuT0ycAymq
# ovwYoCkMDVxcViNX2hyWDcgmNsFEy+Xh5m+J54/pmLVz03jj7aMBPHTlXrxs9iGJ
# ZwXsl521sf2vpulypcM04S+f+fRqOeItBIJb/NCcrnydEfnmtVMZdLo5SjnrfUKz
# Sjs3PcJKeyeY5+JOmxtKVDhqIze+ardI7upCDUkkkY63BC6Xb+TnRbuPTf1g2ddZ
# wtiA1mA0e7ehkyD+gbiqpVwJ6YoNvihNftfoD+1leNExX7lm299C5wvMAgeN3/8g
# BqNFZbSzMo0ukeJNtKnJ+rxrBA6yn+qf3qTJCpb0jffYmKjwhQIIWaQgpiwLGvJS
# Bu1p5WQYG+Cjq97KfBRhQ7hl9TajVRMrZyxNGzBMMYIEjTCCBIkCAQEwgYcweTEL
# MAkGA1UEBhMCVVMxEzARBgNVBAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1v
# bmQxHjAcBgNVBAoTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEjMCEGA1UEAxMaTWlj
# cm9zb2Z0IENvZGUgU2lnbmluZyBQQ0ECCmEF9x4AAAAAADIwCQYFKw4DAhoFAKCB
# ujAZBgkqhkiG9w0BCQMxDAYKKwYBBAGCNwIBBDAcBgorBgEEAYI3AgELMQ4wDAYK
# KwYBBAGCNwIBFTAjBgkqhkiG9w0BCQQxFgQUd3flNhpZutjVQaKf2QosbzL2cEsw
# WgYKKwYBBAGCNwIBDDFMMEqgLIAqAE0AaQBjAHIAbwBzAG8AZgB0ACAAQwBvAHIA
# cABvAHIAYQB0AGkAbwBuoRqAGGh0dHA6Ly93d3cubWljcm9zb2Z0LmNvbTANBgkq
# hkiG9w0BAQEFAASCAQBm8NBVL2TdTa6hUGYakKs7YmxUdvO4FQ9YIzPMFPQSZd9A
# ZjQSOlYbB6CPiJPZB3eA0s4OKhg7BZAknmM7xI9zB7KFk/5uO6jYtaiQe6EjdHPf
# cCdYTBm2rP0F+eKrHJQkGUjjBPXI8t6eQ+3Ku3chm6nCKuH1dpRP7GZTK15wGwkq
# 7TxhGSg9BpopUGYNay/QNnbLo4DNMKkBx5hxiZSH4qvY3zI1jgz/Ir8ke/T8wZgP
# cDTnrSGYM8B45WlDcQowPfa6MLzRya7k5TgAe3YwxaJTAO7zQ5Vw12taGK18pr8p
# IFW2EW/L2RsGxZ3Ze/+TWHTIOgCLz4uir3yZHLmBoYICHTCCAhkGCSqGSIb3DQEJ
# BjGCAgowggIGAgEBMIGFMHcxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpXYXNoaW5n
# dG9uMRAwDgYDVQQHEwdSZWRtb25kMR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9y
# YXRpb24xITAfBgNVBAMTGE1pY3Jvc29mdCBUaW1lLVN0YW1wIFBDQQIKYQTKaQAA
# AAAACDAHBgUrDgMCGqBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZI
# hvcNAQkFMQ8XDTA5MTAyMzA1MjA0MFowIwYJKoZIhvcNAQkEMRYEFOfHFFMMdpC2
# xxuLJdvYmJRYEnDpMA0GCSqGSIb3DQEBBQUABIIBADUBSLQWTy4MgykCSe99Vdx7
# il4NCof1CMEINFI1DIob/vhzu7L1FRpnmkQQFmai1FT8IVR8LHalFoIxdpShNs6Z
# r8dJmHwWlBjq8xhCCu3rTHPdvLZ5qfwyQh67MsK7ah6v9YYdda+Q25iCbR23Tqdn
# yUlCW/8MvO9vNGeJOaKN5epL8R9MzBULU+BB9AguIRBupvSFHpXtI/BqCuKUwzvk
# q767ZUdUWB9CCsfs6Hicj0bK/TkLxc+hPivEkiL2mtOpXPzMDnyy5q0WOUCRKb2z
# ANtV9T4uj8oZevbkXVdc7WNOF9E11AEtvIvg8rLwC58Mztg5guZb9VB7J40rHFs=
# SIG # End signature block
