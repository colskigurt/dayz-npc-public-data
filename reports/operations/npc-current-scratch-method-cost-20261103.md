## 1. 작업 목표

현재 network NPC 코드에서 발 지지·관절 sweep solver의 임시 배열 재사용이 의미 있는 절감 후보인지 최소 통제 비교한다. 정상 밀침·낙하·착지·FOLLOW·재등반을 보존하며 새로운 실제 물리 검사 생략은 하지 않는다. 기존 scratch 후보는 오래된 FCOn/FCEligible 의존성이 있어 그대로 재로딩하지 않는다.

## 2. 실제 수행

현재 서명 PBO8개의 owner/source와 solver override 부재를 감사했다. 원래 solver BASE, 같은 wrapper OFF→super, ON→개체별 raw/상수 offsets 배열 재사용의3팔 비교다. 출력 points는 매회 새로 생성하며 본·발·월드·fallback 검사는 동일하다. 변경을 역변환하면 packed base method와 일치한다. 새 PBO 없이 격리 mission subclass만 추가했다.

두 독립 서버 프로세스에서 terrain/roof/lift_reject의3조건×4그룹×6블록을 수행했다. 첫 실행 BASE/OFF/ON/ON/OFF/BASE, 두 번째 ON/OFF/BASE/BASE/OFF/ON. 블록당64회 준비·2000회 측정, 프로세스당72블록·144000회, 합계144블록·288000회다. 그룹 내 native4본 포즈·root 안정성 및 출력 소유권/forced-busy fallback을 검사했다. 실제 재귀 호출 검증은 아니다. 사전 후속 기준은 두 실행·양 방향 중앙값 모두 최소5% 감소다.

## 3. 측정 결과

|실행|조건|BASE 중앙값 ticks/2000회|OFF 중앙값|ON 중앙값|ON 대 BASE|
|---|---|---:|---:|---:|---:|
|scratch-method-cost1d|terrain|1071379.0|1079114.5|1052671.5|-1.7461%|
|scratch-method-cost1d|roof|1502172.5|1482581.0|1464076.5|-2.5361%|
|scratch-method-cost1d|lift_reject|991792.0|996442.5|972158.0|-1.9796%|
|scratch-method-cost2d|terrain|1120606.5|1142297.0|1099311.0|-1.9004%|
|scratch-method-cost2d|roof|1535389.0|1553402.5|1511889.0|-1.5306%|
|scratch-method-cost2d|lift_reject|1044753.5|1059273.5|1020175.5|-2.3525%|

전 조건 footRays=20000/블록. terrain/roof jointSweeps=8000, lift_reject=0. 그룹 안 반환·lift 합·points 수·질의 수 동일, root/pose/출력 보존 가드 통과. 두 실행 모두5% 문턱 미달이다. 작은 감소 경향을0효과로 단정하지 않지만 full64 시험 후보로 승격하지 않는다.

p50/p95/p99는 첨부 JSON의 runs→conditions→blockTicks와 gameTickProbeSeconds에 있다. 전자는2000회 메서드 블록의 raw 성능 카운터, 후자는 블록 바깥에서 읽은 게임시간 차이다. 프레임 시간·CPU self-time·하드웨어 cycles가 아니다. SDK TickCount는 performance ticks로 정의돼 절대 ms 변환을 가정하지 않았다. 실제 Present/CPU busy/GPU/8·64 기능/시각 품질·할당량·GC는 이번에 미측정이다. 기존8/64 결과를 이번 시험의 회귀로 재사용하지 않는다.20ms p95는 미달성 목표다.

## 4. 원인 분석

확인: 두 script 배열의 수명 연장만으로 관찰된 국소 이득은 약1.5~2.5%이며 native Ray/Sphere 내부 호출은 그대로다. 따라서 이 후보만으로 기존 큰64개체 비용 차이를 해결한다는 근거가 부족하다. 나머지 비용을 전부 물리라고 역산할 수 없다. VM·본·출력 배열·native 질의·스케줄링도 포함된다. native 내부 임시 메모리는 호출자가 제공하는 script 배열과 다르다.

시험 초기에 override 매개변수명 불일치 및 opaque file-handle cleanup 컴파일 실패가 있었다. 후속 no-physics 장수 actor 실행은12행 뒤 native read0 crash가 발생했다. 원인 타입은 미확정이며 가까운 과거 fault RVA만으로 동일 원인이라 하지 않는다. 공식 physics 생성+script retirement 분리 후 두 실행을 완료했다. 실패 원본은 보존하고 성능 표에서 제외했다.

원시 최종 TSV는16개 header/17개 data 필드다. 마지막 gameTickProbeSeconds 라벨 누락을 별도 header-projection.json으로 명시해 파싱했고 원본은 수정하지 않았다. 초기 부분 결과의 elapsedMs 라벨도 틀려 비교에서 제외했다. 과거 scoped-cost 보고서는 이미 clock 보정식이 있으므로 이번 오류를 소급 적용하지 않는다. launch의 newPbo=true는 복사된 runner 메타데이터 오류이며 현재8개 hash 감사와 실제 생성 이력상 새 PBO=false다. 원시 launch는 보존한다.

## 5. 검증 수준

방법 진단 및 두 독립 프로세스의 제한된 반복 비교 완료. 후속5% 기준 미통과. 전체 게임 기능 회귀/시각 수용/성능 목표/일반 안전성은 미실행·미수용이다. 프로세스는 종료했고 소유 서버 정리 exit-1은 정상 quit 인증이 아니므로 normalQuitVerified=false다. 전체 골은 active다.

## 6. 변경 사항

mission subclass·staging/analysis/report 파생 자료만 추가. 기존 source/PBO·자산·운영 서버·graph/native 시계·지원 판정은 변경하지 않았다. 새 PBO 빌드 없음. 실행 전 기존8개 서명과 hash 검증 성공. 개인키는 공개하지 않는다. buildId는 JSON의8개 PBO+실행파일 조합이다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/network-gameplay/current-scratch-method-cost. 1d/2d의 plan·init.c·launch·TSV·RPT·header-projection과 analysis, 초기 컴파일 실패/부분 crash를 로컬 checkpoint 해시 목록에 연결한다. 공개 패킷은 선택된 수치와 자체 mission wrapper 발췌이며 전체 원시자료·게임자산·영상·로컬경로·프로세스·키는 제외한다. 팔별8블록은8개 독립 프로세스가 아니다.

## 8. 잔여 문제

이 후보는 보류한다. 다음 단일 산출물은 현재 client/server topology에서 native Ray/Sphere 상위 호출의 안전하게 줄일 수 있는 중복을 기존 증거로 좁힌 수정 후보 명세다. 질의 삭제·항상 참 처리·무조건 캐시를 적용하지 않는다. 동일 입력 재사용 빈도 미확정 및 이전42쌍 완전일치0을 유지한다. 수정 후보가 없으면 제한을 명시하고 지형 안전 판정과 무관한 broadphase/스케줄링만 별도로 검토한다. 표시 품질과64개체 비용은 독립 게이트로 유지한다. 현재 표시 개선은 사용자 피드백이며 모든14클립·8파트·64개체 최종 수용은 아니다. 새 ETW 표시 측정에는 유효한 새 관리자 준비가 필요하며 오래된 helper는 재사용하지 않는다.

로컬 checkpoint SHA256: d67c1200dc263d90d84a190bf5c145ff6b1d75be381a7631ad4672a4fef04a8c
