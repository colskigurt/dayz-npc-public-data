# 실제 Present·표시 주기와 NPC 명령 진행 분리: 첫 ETW 측정

작성: 2026-10-31T17:48:47.847221+09:00

## 1. 작업 목표

실제 Present·표시 간격을 CPU 미션 콜백 및 NPC 명령 진행과 구분해 측정한다. 앞선 ETW 권한 제한을 해소하고, 실제 표시 주기를 확보하되 특정 감염자 포즈의 GPU 소비나 시각 품질까지 증명한 것으로 확대하지 않는다.

## 2. 실제 수행

사용자가 관리자 창이 보이지 않는다고 알려 실행 상태를 확인했다. 준비 기록과 해당 실행 프로세스가 없었으며, 기존 바로가기는 NoExit가 없어 조기 오류가 보존되지 않을 수 있었다. 원래 단일 측정 스크립트는 보존하고, transcript·오류 JSON·NoExit가 있는 표시용 wrapper로 바로가기를 보완해 실행했다. Windows UAC를 자동 클릭하지 않았다. 이후 관리자 준비 기록과 실제 실행 프로세스를 확인했다.

그 뒤 기존 서명된 B8FB 고정1개체 run 시험판을100초 실행했다. 실제 전경·포즈조회·본갱신 시작을 확인하고, 현재 PID/profile/build를 검사한 일회용 관리자 측정기로30초 ETW를 수집했다. PresentMon은 고유 세션을 정상 종료했고 게임도 정상 종료했다. 키보드 추적·영상·Frida/native hook·numeric 보정은 없었다. 준비 단계의 권한 거부 기록은 과거 사실로 보존하며 이번에 해소됐음을 구분한다.

실제 CSV에는 DisplayedTime이 없고 TimeInQPC·MsBetweenDisplayChange·MsUntilDisplayed가 있었다. 최초 준비 분석기는 형식 차이를 거부했다. 원시CSV와 기존 분석기는 수정하지 않고 별도 분석기에서 실제 열의 의미를 검증했다. Present QPC 차이·CPU start의 다음 구간·표시시각 계산이 각 CSV 지표와 일치하는지 검사했다.

## 3. 측정 결과

실행 runtime-20261031-174120, 단일 logical NPC, 기존8파트 진단 자산, AI/이동OFF·run·VSyncON·새영상없음이다. 한 DXGI swap chain에서1495행, 실제 Present 범위29.8804초를 수집했다. 모든 행의PID를 검증했고 swap chain을 합치지 않았다. PresentMode는1495행 모두 Hardware Composed: Independent Flip, SyncInterval=1이었다.

| 측정 범위 | p50 ms | p95 ms | p99 ms |
|---|---:|---:|---:|
| Present 호출 간격 | 19.0927 | 28.4305 | 32.2584 |
| 표시 변경 간격 | 20.0048 | 20.0851 | 20.1149 |
| Present→표시 지연 | 54.0260 | 59.7032 | 59.7782 |
| 프레임 CPU busy | 10.7429 | 15.8107 | 16.8498 |
| 프로세스 GPU busy | 3.3306 | 7.5975 | 9.3405 |
| GPU 구간(대기 포함) | 20.0072 | 21.3433 | 22.0492 |

MsUntilDisplayed·MsBetweenDisplayChange 누락은0행이었다. 손실 경고는 없었지만 ETW lost-event counter를 직접 측정하지 않아 event loss=0으로 수용하지 않는다. MsAnimationError는1494개 유효값이며 NPC 본포즈 시간이 아닌 CPU/display frame delta 차이다. MsBetweenSimulationStart와 MsFlipDelay는1495행 모두NA로 미측정이다.

독립 nonce/ACK39건의 clock offset 교집합 폭은12.62ms다. 이 불확실성을 보수적으로 적용해 ETW 창 안의 전경CPU callback1493개를 선택했다. callback 간격 p50/p95/p99=20.2601/34.5562/36.4687ms, 전경비율1.000이다. 선택창29.8341초에서 미션 개체 NativeSeconds 증가19.49989초·AnimUpdates 증가390회, 명령 시간/벽시계=0.653612, callback 갱신률13.0723회/초였다. 이 Script 열은 각 렌더 파트의 직접 graph clock 측정이 아니다.

Present QPC와MsBetweenPresents 최대 차이는 약1.5e-14ms, CPU QPC와다음MsBetweenAppStart·계산한표시시각차이와MsBetweenDisplayChange는 출력정밀도에서0ms였다. 이는열해석 검증이며 ETW trace 완전성이나 특정NPC 표시시점 증명은 아니다.

## 4. 원인 분석

확인: 이 단일 조건에서 실제 표시 변경 간격은약20ms로 일정하지만 Present 호출·CPU callback의 시간 간격은 더 넓게 흔들린다. 같은 창에서 NPC 미션 명령 시간의 평균 진행은약0.654배였다. 따라서 화면 전체의 표시 FPS와 NPC 갱신 진행을 같은 문제로 취급하면 안 된다. 앞선 source-FK proxy 교정·native 시간회계 근거와 별도 축의 관측이다.

미확인: 표시된 각 frame에 어느NPC 포즈가 소비됐는지, GPU palette/upload/skin self-time, 최종 부드러움이다. Clock offset12.62ms는 단일 frame의 CPU/Present 대응을 확정하기에 크므로 이 자료를1:1 pose join으로 사용하지 않는다. GPUTime약20ms는대기포함 구간이며감염자 스키닝20ms라는뜻이아니다. GPU busy도 전체대상프로세스 범위로개별NPC 귀속이아니다.

이전 녹화·Frida진단과 이번 ETW 실행은 계측·실행조건이달라 차이를최적화개선이나특정도구overhead로차감하지않는다. 이번 결과로사용자 관찰의끊김이없다거나GPU가항상병목이아니라고결론내리지않는다.

## 5. 검증 수준

권한해소·실제ETW수집·PID/swap-chain/열수식 검증·안전종료:단일진단성공. 반복수용·NPC GPU포즈/실제시각품질·64기능회귀·64성능·최종수용:미완료.1개체표시p95를64개체20ms목표달성으로판정하지않는다.

## 6. 변경 사항

관리자창의가시성wrapper/바로가기보완·일회용측정요청·분석/보고파생물만추가했다. 게임Source·미션·PBO·운영서버·키·이전원시기록은보존했다. 실행전기존3모드서명검사성공,새PBO/서명생성없음이다. 관리자측정기는선택PID만출력했고다른세션중단·서비스설치·권한그룹변경·게임자동종료를하지않았다. 측정완료후관리자대기창은사용자가닫아도된다.

## 7. 원시 증거

plan/readiness/request/capture-link·PresentMon CSV/종료/stdout/stderr·CPUclock/nonceACK·launch와도구출처·wrapper를연결한다. 공개패킷은익명화한chain alias·상대Present시각·선정행·집계이며로컬경로·PID·heap주소·절대QPC·영상은노출하지않는다. 원시CSV와이전실패분석은보존했다. 게임/측정기종료후파일안정성·JSON·hash를검증해저장한다.

## 8. 잔여 문제

실제표시 cadence라는측정축은확보했지만반복검증과NPC포즈소비연결이필요하다. 후속측정에서독립clock bracket을더좁히거나실제pose/frame동일성을확보하기전에는새보간을적용하지않는다. 이번단일측정의GPU/CPU busy를64비용기여도로일반화하지않는다. 별도로진행한고정지지guard비용시험은그자료만으로분석하며현재capture와동시측정하지않았다. 정상낙하·착지·FOLLOW·재등반과기존64엄격자극비교미수용을유지한다.

선정공개본문.로컬보고서SHA-256:`0e644bbdae921915ab19f3a8b47633f205384c3b2fadfabc40b9b21dc928ce92`.전체raw가아니다.
