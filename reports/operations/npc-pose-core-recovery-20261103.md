## 1. 작업 목표

앞선ID24 진단에서 확인한 일반4관절 solver와 복구용 root 고정3몸통 구의 허용공간 차이를 국소 후보로 해결할 수 있는지 검증한다. 기존 방향16개·1cm걸음 축소만으로는 해결되지 않았으므로 같은 방법을 반복하지 않는다. 정상 밀침·낙하·착지·추적 재개·재등반은 유지하고 기능·시각·성능을 별도로 판정한다.

## 2. 실제 수행

기존 FRAME_FALL_CONTRACT·RECOVERY_E6D3·2026-10-28 착지 corridor·2026-10-29 포즈별 경로·2026-10-30 actor60 방향 연구와 packaged refinement wall/footprint guard 코드를 읽었다. 벽·발 지지·기존 겹침의 비심화·native sweep 유지 계약은 확인했다. 고정3몸통을 처음 추가한 정확한 변경 이력/최초 의도는 확보하지 못했다. 현재 코드를 읽어 도입 이유를 확정한 것처럼 기록하지 않는다.

후보V1은 기본OFF이며, 발판에 지지된 FOLLOW+PoseClearanceActive+등반/진입정렬 잠금 없음+locomotion 요청0~2에만 적용한다. 지면 ground뿐 아니라 동일한 실제 지지·area 조건을 만족하는 low/high도 포함한다. 낙하·착지·MOTION·Active·EntryAligning·공격·지지 부적합은 원래 predicate로 위임한다. 반경0.23m의3개 구를 유지하되 중심을 양발 평균↔골반 중간,골반,골반↔머리 중간으로 옮겼다. 기존4관절·peer/core·예약·terrain foot lift·fixture 비심화·native sweep 규칙은 남겼다. 포즈 계산은 동일8개 old/new joint 결과를 재사용하여 중복 getter를 늘리지 않았다. 지원 조건 판별에는 Floor 조회가 추가되므로 비용 절감 후보로 주장하지 않는다.

기존3개의ID24 frozen 입력으로 원래 dt0.05를 양쪽에 동일하게 제공했다. 별도3cm 유지 후보를 mission-derived class로 비교한 뒤 같은 소스를 기본OFF 모듈로 포장해 컴파일·flag 부재·22개 상태/벽 guard·기존 복구 모터를 다시 검사했다. 새 PBO를 빌드·서명·서명검증·packed script 일치 검사했다. 그 뒤 동일 모듈로8명→64명 네트워크 기능 시험을 실행했다. 운영 배포본/기존6PBO는 수정하지 않았다.

Computer Use로 각 실제 client 장면을 확인한 뒤 기존 start gate를 열었다. 새 영상·native observer·ETW는 사용하지 않았다. 8명 client 시작 전 DsCheckSignatures가 경로 인자를 처리하다 대기했다. 소유 signer child가 살아 있고 client 미생성임을 확인해 그 child만 종료했다. 기존 서버는 재시작하지 않았다. 원래 launcher/미사용 profile을 보존하고 Join-Path로 인자를 정규화한 별도 client launcher·새 profile로 진행했다. 정규화 실행의 서명 검증과 client 시작은 성공했지만, tool 대기의 내부 원인을 단정하지 않는다. 임의로 종료한 signer의 반환을 자산 서명 불량으로 해석하지 않는다. 이 준비 지연 때문에8명 native warmup과64명 조건은 같지 않다.

## 3. 측정 결과

|항목|실제 결과|범위|
|---|---|---|
|ID24 원래 fixed-core|3포즈 모두 이동0m|frozen 동일 root, 독립3회 실행 아님|
|ID24 pose-core 후보|각10걸음·약0.302737m, 최종 일반 접촉 통과|같은 dt0.05·기존 모터, 고정 포즈|
|packaged 후보 parity|같은6 motor행·22 guard행 확보|원래 delegated wall 거부 포함, 새 pose-envelope 벽 전체 안전성 인증 아님|
|기본OFF guard|flag 없음·cold-read에서 원래 거부 유지|해당 profile 범위|
|8명|8/8 모든 milestone·유효 종료,9/9 낙하 복귀|67.2037초·144전이, 단일 실행|
|8명 후보 실제 사용|0회|기능 회귀만, 후보 효능 증거 아님|
|64명|64/64 낮은/높은/하강/재등반 milestone·유효 종료|289.754초, 단일 실행|
|64명 낙하 복귀|91/91 FALL→LAND→FOLLOW|정상 낙하를 실패로 제거하지 않음|
|64명 상태 계약|1,294전이, phase chain/generation/terminal mismatch0|기존 gate 유지|
|64명 후보 사용|18개체·321회|predicate 사용량, accepted-step 수나 CPU시간 아님|
|추가 관찰|20.069초, 마지막5초 무이동+접촉거부 선별0개체|표본 기반 정체 선별, 모든 미래 정체 부재 보장 아님|
|64명 server Mission dt p50/p95/p99|47.5/69/81ms|Present/CPU busy/GPU/애니메이션 표시 cadence 아님|

새 실제 Present·CPU busy·GPU p50/p95/p99·할당/GC·전체14클립 실제 표시 부드러움은 미측정이다. 20ms p95는 목표이며 달성으로 보고하지 않는다. 앞선 EGRESS-only의63/64 high와 이번64/64의 차이를 이 후보의 순수 인과 효과나 성능 개선률로 계산하지 않는다. 초기 포즈 위상/날씨/start gate·군집 궤적이 같게 고정되지 않았다.

## 4. 원인 분석

확인:동일 frozen ID24 입력에서 고정 root 몸통을 포즈 기반 같은 반경3몸통으로 바꾼 것이 기존 검사/모터로 이동을 만들었다. 일반 solver가 이미 허용하던 움츠린 포즈를 upright root 몸통이 추가로 막는 불일치를 국소적으로 설명한다. 원본추출·graph 시계 수정으로 해결한 사례가 아니다. 실제64에서는 후보가 발동했고 기능 gate를 통과했지만 이 사실만으로 전체 결과의 인과를 확정하지 않는다.

추가 발견:packed 원본453D의 PressureEscapeStepClear와 소스는 동일했다. 하지만 B8 모듈에 DGSubsetTerrainContact의 기존 override도 있었다. terrain-contact-owner.flag가 없으면 super로 위임하므로 이번 시험의 기본 물리 규칙은 원래 base와 같았다. 확인 없이 ‘다른 override가 없다’고 가정한 첫 source 검증은 실패 처리하고 보존했다. 별도 owner-chain 분석과 수정된 proof-v2로 실제 wrapper 및 profile의 flag 부재/mission 직접 강제 없음을 확인했다.

호환성 문제:V1의 직접 후보 method는 기존 terrain owner가ON일 때 그 정책을 우회할 수 있다. 이번 시험은 ownerOFF 범위에 한정하며 일반 적용은 미수용이다. 이를 막는 V2 소스를 별도 폴더에 준비했다. 원래TCPolicyRead/TCEnabled/TCTrace 초기화와 같은 flag 정책을 읽고 TCEnabled면 원래 super override에 위임한다. ownerOFF의 pose geometry/native predicate는 문자열 역변환으로 V1과 동일함을 확인했다. V2는 아직 빌드·서명·엔진/기능 검증하지 않았다.

미확인:새 pose envelope가 실제로 사용되는 벽/발판 음성 대조, 포즈 간 몸통 sweep 연속성,극단적인 locomotion 포즈와 다른 collision 형상,전투/피격의 동일 hash 회귀가 남는다. 기존 fallback에서 wall 거부를 유지한 guard를 새 몸통 범위의 전체 벽 안전성으로 확대하지 않는다. 추가 Floor 질의의 실제 비용도 미측정이다.

## 5. 검증 수준

fixed-core 대 pose-core 고정 입력:진단 성공. V1 패키지/기본OFF/상태 guard:해당 범위 통과.8명 및64명 기존 이동 시나리오:각 단일 회귀 통과.91회 정상 낙하 복귀 보존 확인. 반복 검증 완료·일반 적용·V2 owner 호환성 runtime·시각 품질·성능 최종 수용:미완료. functionalGatePassed와전체functionalAccepted를 구분한다. 전체 골은active다.

## 6. 변경 사항

격리 모듈 nf_pose_core_recovery V1 PBO SHA256=5C43ED24F404020D573E7293F43B6C21B890A42D6C468E5C1901EA2AC93A60D5. 빌드·자동 서명·DSCheckSignatures·packed script/source 일치 성공. 실행 전 기존6PBO와 새1PBO의7개 해시/서명 검증 성공. builder metadata의 engineCompiled=false/runtimeVerified=false는 빌드 시점 표식으로 보존하며, 이후 packaged/8/64 엔진 실행 증거와 구분한다.

새 source/staging·frozen/package/eight/sixtyfour 격리 mission·원시 TSV/분석·V2 source-only·보고서만 추가했다. 원래6PBO·애니메이션 자산/그래프/native 시계·운영 서버·Steam 게시본은 그대로다. 원시/과거 보고서를 덮어쓰지 않았다. server 완료 후 harness가 소유 프로세스를 정리했고 client exit-1이므로 normalQuitVerified=false를 유지한다. 개인키는 release/공개물에 없다.

## 7. 원시 증거

sourceRunId 묶음:network-gameplay/{actor24-pose-core1,pose-core-candidate,actor24-pose-core-packaged1,eight-pose-core1,sixtyfour-pose-core1,pose-core-candidate-v2}. source-change/owner-change/packed-member/기존 override-chain proof,실제 build/sign log·launch·정확한 known-warning guard·frozen/mode guard·gameplay/milestones/state-contract/terminal·pose-core-use·server-frame/part-clock·시작 전경 확인·client preflight 실패/정규화 launcher·분석 JSON을 로컬 해시 목록에 연결한다. 증거141개 이전 진단은 기존 report/hash로 보존한다. 이번 public packet은 선택 집계·자체 후보/기존 owner 발췌이며 원시 전체·게임 자산·영상·로컬 절대경로·프로세스 정보·개인키는 공개하지 않는다.

모든 대상 게임/recorder 프로세스 부재와 launch terminal을 확인한 후 file 안정성·JSON·이중 hash 검증으로 저장한다. 시험 도중 보고서를 만들기 위해 중단하지 않았다. 실제HTTP/hash 및 이번 공개 JSON supplement schema 검증은 별도 publication proof에 기록하며 external AI 새 읽기는 수행하지 않았다.

## 8. 잔여 문제

다음 가장 높은 가치는 V2의 기존 terrain owner ON/OFF 위임을 packaged guard로 입증하는 것이다. V1 기능 성공을 이유로 이 충돌을 건너뛰지 않는다. 새 pose-core가 실제 쓰이는 벽·발판 음성 대조도 추가하고, 기존 mode/action guard와8→64 regression을 근거 범위에 맞춰 유지한다. 후보는 기본OFF·격리 상태이며 운영/Steam 반영은 하지 않는다.

비용과 애니메이션 문제는 미해결이다. 이 기능 수정은 중복 물리 질의 최적화가 아니며 Floor 조건 조회가 추가된다. feature 안정화와 별개로 동일 평지·카메라·반복 조건에서 기존 안전한 비용 후보를 비교해야 한다. 기존network 표시 개선과 이번 이동 안정성을 섞어60fps/20ms 성공으로 기록하지 않는다. 실제 ETW 비용 비교는 현재 관리자 승인 경로가 필요하고 이번에는 실행하지 않았다.

로컬 checkpoint SHA256: 15599885f34427ce63137b4aba9b14adc496483d25184326768cd195dc47d763
