# 경계 재사용 Update/Apply 및 실제8 shadow

## 1. 작업 목표

정적 경계 재사용 후보의 실제 Update/Apply 동등성 및 실제 8개체 shadow 결과를 검증한다. 기능·시각·전체 비용을 독립 판정하고 기존 5파트 빌드와 밀침·낙하·착지·FOLLOW·재등반을 보존한다.

## 2. 실제 수행

같은 서명된 default-OFF NFBoundsEpoch 애드온을 사용했다. 원본 StaticMap.Update/Apply를 16 metadata body와 정적 물체35개에서 실행하여12단계 비교했다. NPC 엔티티는 생성하지 않았다. 최초 실행은 완료마커만 있고 TSV헤더만 있어 증거불충분으로 제외했다. 시험 미션의 단계명과 명시 행 기록·진입 횟수 확인만 수정한 재실행에서12행을 확보했다. 최초 누락의 엔진 내부 원인은 미확정이며 완료마커만으로 통과하지 않았다.

이후 기존 8개체5파트/40호스트 mobility 미션을 복사하고 최종 Bounds 계수 로그만 추가했다. 신규 bounds 애드온만 추가, 과거 미수용 rank 애드온 제외, 입력 씬·경로/물리 코드는 유지했다. enabled=true,shadow=true로 후보 값을 계산·대조하되 실제 게임에는 매번 원래 Bounds 결과를 사용했다. 새 프레임 비용 수집은 하지 않았다. Steam 일반 권한 클라이언트와 기존 실행기를 재사용했다.

## 3. 측정 결과

| 항목 | 관찰 |
|---|---|
| 실제 Update/Apply | 12단계 불일치0,16 metadata body·35물체 |
| 경계 재사용 | 1326 hit / 288 miss |
| 장애물 갱신 | 기준·후보 모두 68 refresh |
| 실제 8개체 shadow | 유효 종결 8/8,전 구간 이정표 8/8 |
| 낙하/착지 후 FOLLOW | 시작 8,복귀 8 |
| 상태/세대/종결 정합성 오류 | 0/0/0 |
| 실제 Bounds | 4059 calls,0 hits,68 misses,0 mismatches,532 epochs |
| 활성 시나리오 | 59.4575 게임초,단일 실행 |
| 신규64·Present p50/p95/p99·CPU/GPU | 미측정 |

Update 비교는 장애물 min/max 배열,signature,localCount,path,PlanAt,Approach flags,Cursor,NextUpdate,epoch 후 캐시 해제를 포함한다. 미도래 갱신과 동일 signature의 경로 보존도 확인했다. 이동/회전/크기변경·동적상태·삭제/null·고도/거리 필터·고정 장애물 변경을 포함했다. 이 metadata 시험의 Scan은 기존 cell-age 캐시를 prime하여 Queries=0이므로 새 발견 검증은 아니다. 실제8 shadow에서는 기존 Scan/물리 검사를 그대로 실행했다.

## 4. 원인 분석

확인된 사실: 실제 Update 경계에서만 후보 캐시가 활성화되고 종료 후 비워졌다. 대상12단계 Apply 결과는 동일했다. 실제8개체에서는hit=0이므로 shadow 대조 경로 자체가 실행되지 않았다. 불일치0을 캐시 사용 안전성 증거로 간주하지 않는다. 가설: 반복된 정적 물체 경계 계산의 비용을 줄일 수 있다. 아직 미확인: 후보 캐시 결과를 실제 동작에 사용하는 ON 시험,64개체 전체 CPU/Present 순이익,장시간·모든 물체 종류·동시 변형 안전성. shadow 결과는 원래 계산을 계속 사용하므로 실제 ON 기능 회귀 통과를 대신하지 않는다.

기존 함수 범위 약49~53% 감소는 전체 프레임 절감률이 아니다. 최초 전원생존 장면은 정적 기록이 적으므로 주요 Ray/Sphere 비용 해결로 확대 해석하지 않는다. 과거 rank 조기종료의 전체 비용 세 쌍 미수용 판정을 유지한다.

## 5. 검증 수준

Update/Apply 제한 조건: 진단 성공. 실제8 shadow: 캐시사용 경로 미실행,단일 실행의 기존 기능 gate=True. 실제 cache-ON8/64 기능: 미실행. 시각 품질: 이번 스크린샷으로 시간적 부드러움 판정 불가,미측정. 전체 성능·20ms·최종수용: 미완료. 서버 actual exitCode=-1을 정상프로세스 종료로 수용하지 않는다. 클라이언트 종료 증거는 별도 파일로 보존한다.

## 6. 변경 사항

신규 시험 미션·입출력 manifest·선정 분석/보고서만 추가했다. PBO는 직전 서명/검증 성공한 동일 SHA `114417D39AA89921AC50B550F98A810768AEF3EC197AD759AED42E8887F85037` 사용,새 재빌드 없음. 원본 게임 코드·PBO·모델·운영 서버 변경 없음. 신규 애니메이션 clock/보간/충돌 삭제 없음. 개인키 공개/포함 없음.

## 7. 원시 증거

실행 ANIM-bounds-update1(증거불충분·제외),ANIM-bounds-update2(12행확보),ANIM-bounds-eight-shadow1(실제8 shadow). 기존 pinned state-contract analyzer의 판단 로직을 I/O adapter로 재사용했다. 공개 packet은 실제 시험 미션/최종카운터/선정 상태전환·종결행과해시만 포함하며 전체 원시로그·영상·자산·개인경로·프로세스정보를 공개하지 않는다. 이미 보고한 함수비용 자료의 분석 재사용을 새 독립 반복으로 세지 않는다.

## 8. 잔여 문제

실제8개체의hit=0으로 같은 씬의cache-ON 반복은 우선 보류한다. 이 후보는 시체·소형 정적 물체가 많은 후반 조건용으로만 남기고 전원생존 주력 후보에서는 낮춘다. 다음 단일 작업은 기존 Drive/Displace 호출소유·횟수 증거에서 한 논리갱신의 중복 솔버 실행을 안전하게 줄일 수 있는지 확인한 결정표다. 인접 입력 완전일치0인 기존 결과를 재사용하고 무조건 캐시를 제안하지 않는다. 다음 Present 측정 전 기존 수집기의 exitCode 누락을 최소수정·검증해야 한다. 기존 정체 복구 반복·시체 착지 대기는 별도 결함으로 남기고 이번 작업을 기능수정으로 전환하지 않는다.

## 후속 계측기 수정 — 새 게임 실험 없음

현재 통합 계측기의 종료코드 누락 대응3조건 회귀 통과. 새 ETW/NPC 성능 측정 없음. 기존 단순 결합솔버 기각과 실제8 Bounds hit0 보류 유지. [상세](reports/operations/collector-exit-contract-20261107.md) · [근거 TXT](evidence/collector-exit-contract-20261107.txt) · [JSON](api/public/collector-exit-contract.json)
