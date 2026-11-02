## 1. 작업 목표

기존1NPC network GroundBody 연결 후보를8NPC로 확장해 접촉·등반·낙하·착지·재등반과 상태 계약을 검증한다. 각NPC8파트로 총64native parts이며64NPC 시험이 아니다. 이전 골 턴은 실제1NPC 이동·재인식/재등반과 새 서명 격리 모듈을 확인한 progress였다.

## 2. 실제 수행

이전 후보765E PBO와 기존5PBO·동일 진단 server/client를 그대로 사용했다. 원래 인식·공유 보고·경로·Drive/Displace/지지/관절 검사·낙하·착지·Presentation commit을 유지했다. 새 PBO·그래프·시계 수정·collision layer 변경·관측기·녹화는 없다.

기존 fixture에서 실제 플레이어 위치 자극을 낮은 지붕→높은 지붕→지면→앞쪽 시야→지면 유도→낮은 지붕 순으로 제공했다. 각ID의 낮은/높은 지붕 완료·지면 FOLLOW 복귀·재등반을 누적했다. 정상적인 지지 상실/낙하가 있더라도 복귀 진행을 인정했다. 모든 개체가 좁은 지붕에 동시에 서야 한다는 조건은 두지 않았다. 목표/기억/NPC 논리 위치에는 값을 주입하지 않았다.

기존 state-contract.flag로 실제 SetPhase 전이·복구 generation을 기록했고,퇴역 전에 각ID의 최종 상태·실제 Floor 지지 판정 결과·milestone 결과를 별도 terminal TSV로 저장했다. 이전1NPC 시험의 마지막 TSV가 terminal 직전 상태였던 공백을 이 묶음에서 보완했다. 이 기록은 격리 profile에 추가한 원시 증거이며 새 진행 DB가 아니다.

eight2/eight3은 새로운 server/client 프로세스·프로필·포트에서 각각 실행했다. mission 바이트/PBO/실행파일/count/fixture/자극/계약 flag는 같지만 초기 idle 위상·전경 시작까지 대기·날씨는 고정하지 않았다. 동일 위상 성능 A/B가 아니다. Computer Use로 시작 전 실제 창을 활성화했고,eight3 실행 중 높은 지붕의 여러 개체와 낮은 곳의 한 개체가 함께 보이는 스냅샷을 확인했다. 정지 화면으로 부드러움을 판정하지 않는다.

## 3. 측정 결과

|항목|eight2|eight3|
|---|---|---|
|논리NPC/native parts|8/64|8/64|
|active 구간 종료 시각|70.5493초|78.864초|
|milestone 완료/유효 terminal|8/8·8/8|8/8·8/8|
|희소 표본 행|2184|2440|
|기존 계약 행/전이|229/144|229/144|
|기록된 전이 체인·복구 generation·최종상태 대조 오류|0/0/0|0/0/0|
|FALL 시작/후속 LAND·FOLLOW 확인|9/9|9/9|
|비예정 지지 상실|ID0·1회|ID0·1회|
|계획된 하강 FALL|8회|8회|
|historical EGRESS_STALL|ID0·1회|ID0·1회|
|미복귀 FALL·최종 실패|0|0|
|player contact 카운터 합|382|730|
|entityContactFalls 카운터 합|0|0|
|pre-commit lifetime/post-commit 파트 최대 거리|기록 정밀도상0m/0m|기록 정밀도상0m/0m|
|8×8명령 retirement detach|확인|확인|

두 실행에서 terminal phase는 모두FOLLOW·area low였다. 이는 관찰된 결과이며 acceptance가 전원 동시 같은 지붕을 요구한 것은 아니다. eight2는8명 CHASE,eight3는7명 CHASE·1명 INVESTIGATE였다. 실제 terminal 상태와 감각 상태를 구분한다. LOCAL_VISUAL과ALLY_REPORT가 기록돼 직접 시야가 없는 개체도 공유 정보를 사용한 경우가 있었다.

contactSolves 합은36358/42051,contactFailures 합은133/129였다. 이들은 회복 가능한 거부를 포함한 누적 카운터이며 독립 실패 episode나 CPU self-time이 아니다. 같은 CONTACT_SUPPORT_LOST가 FALL/LAND/EGRESS에 이어 기록된 것을 여러 번의 낙하로 세지 않았다.

서버는DONE 후 harness 강제 정리,클라이언트는강제종료false·exit=-1이었다. normalQuitVerified=false다. 새 p50/p95/p99·Present·CPU busy·GPU·할당/GC·전경 유지 비율은 미측정이다.20ms p95는 목표로 남긴다.64NPC·512parts는 아직 실행하지 않았다.

## 4. 원인 분석

확인: network8파트 생성 후보에서 실제8NPC의 기존 이동/접촉/복구 제어가 두 독립 실행의 정의된 경로를 완료했다. 원래 same-NPC28쌍 차단만 사용했고 서로 다른NPC 또는 월드/플레이어 layer를 끄지 않았다. 기존 물리 질의는 계속 실행됐다. 기록된144+144 전이의 prev/next·FALL generation 증가와 terminal phase/gen을 대조했다. 이를 모든 내부 상태의 완전 계측으로 일반화하지 않는다.

확인: 두 실행 모두ID0이 low에서 CONTACT_SUPPORT_LOST로FALL에 진입했고 LAND 후 EGRESS_STALL이 한 번 발생했다. 이후 재탐색/재등반을 진행해 최종실패가 해소됐다. historical failure를 지우거나 무오류 성공으로 재명명하지 않았다. 낙하 원인이 개체 간 밀침인지,발판/포즈/지지 경계인지 이번 자료만으로 확정하지 않았다. entityContactFalls=0이므로 타NPC 밀침 낙하의 직접 재현으로 보고하지 않는다. playerContacts는 관련 경로 실행을 뜻하며 밀침의 시각적 무게감이나 native impulse 자체를 검증한 값은 아니다.

확인: 초기 eight1은 helper 인자 event가 Enforce 예약어라 mission 컴파일에서 거부됐다. eventName으로만 바꿨고 실패 프로필을 보존했다. 준비 실패를 기능 독립 반복에 포함하지 않는다. 알려진 PluginConfigDebugProfile의 정확한8줄 오류만 기존 guard로 구분하고 다른 SCRIPT(E)는 허용하지 않았다.

가설/잔여 위험: 추가7물리 host·복제·표시 보간이64NPC에서 비용을 늘릴 수 있다. 현재8개체 기능 통과는 그 비용을 입증하지 않는다. idle 위상/초기 대기의 차이도 있으므로70.5→78.9초 차이를 후보 성능 변화로 귀속하지 않는다. actual frame/그래프 clock을 다시 후킹하지 않았고,CPU/state 변화와 화면 품질은 계속 별도로 판정한다.

## 5. 검증 수준

8NPC의 정의된 경로·정상 낙하 복귀·누적 재등반·명령 퇴역: 두 독립 실행 범위 기능 반복 검증 통과. 기록된 전이/gen/terminal 대조: 진단 성공. ID0 historical EGRESS_STALL은 복구됐지만 근본 원인이 제거된 것은 아니다. 타NPC 밀침 낙하·공격/피격/시체·14클립 화면 품질·성능 개선·64NPC 회귀·최종 수용은 미완료다. 전체 골은active다.

## 6. 변경 사항

새 자료는 stage_network_eight.py,eight1~3 격리 mission/런처/프로필,analyze_eight.py,분석/보고서뿐이다. 기존 원본/PBO·운영 서버·Steam·애니메이션 자산·native 시계·LOD·솔버 알고리즘은 변경하지 않았다. 새 PBO가 없어 새 서명 대상은 없다. 기존6PBO hash와서명을 실행 전에재검증했다. 사용한 network PBO SHA-256=765E61B0B2E20EA3C7977FF17316808D99D8CDD94DDFBCE7A95F8829CAD6EED8. 개인키·게임 자산·원시영상은 공개하지 않는다.

계약 로그는기능진단용이다. 그I/O가포함된실행을비계측성능baseline으로쓰지않는다. native actor clocks와frame-time/Present domain을혼동하지않는다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/network-gameplay/eight2 및eight3. 각gameplay-eight/milestones/native-state-contract/terminal-state TSV,plan,서버/클라이언트launch,RPT/script,시작전경marker,분석JSON과mission SHA를로컬무결성목록에연결한다. 같은바이트mission과각독립프로세스실행을확인했다. 원본로그·실패프로필·이전보고서는덮어쓰지않는다.

공개packet은선택집계·9회FALL목록·복구타임라인·historical failure·최종상태와자체terminal 판정함수를제공한다. 전체원시로그/절대경로/프로세스정보/키/원본자산은게시하지않는다. 스냅샷은도구대화에서확인했으며별도원시영상으로저장하지않았다.

## 8. 잔여 문제

64NPC 확대에서는같은좁은지붕의동시점유를통과조건으로추가하지않는다. 각ID의유효도달·정상낙하→지지/FOLLOW복귀·재등반진행을누적하되최종미복귀FALL·무효전이·해소되지않은EGRESS를실패로남긴다.8→64는기능확대와비용측정을구분하고,추가물리host/복제로서버CPU로비용이옮겨간것을전체최적화로오인하지않는다.

현재가장높은가치는64NPC 확대의실행가능비용과정상복구를검증하는것이다. 실제Present/CPU/GPU p95에는기존ETW 경로의권한/프로세스상태를재확인해야한다. 오래된admin-ready파일을live worker로취급하지않는다.8시나리오의I/O포함카운터만으로성능을계산하지않는다. 기존scratch/SupportFast 비교는별도실험으로유지한다.

ID0의반복EGRESS_STALL은원시계약에서보존하고,추가질의입력자료가구체적인국소수정판단을바꿀때만기존캡처경로로수집한다. 좁은발판에서의정상낙하를없애기위해충돌/지지를강제로성공시키지않는다.20초시각미리보기에관한사용자피드백은질문을제시했으나이보고서작성시답변미수신이며시각수용근거로사용하지않았다.

로컬 checkpoint SHA-256: 91af29c1e987ce7d7b5e68713acdaf1070f04cf15ea68b8ca13f9be41db84c4d
