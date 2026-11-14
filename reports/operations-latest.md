# Dark Horde 패키지 내부 코드 검토와 LG 확보 실패

## 1. 작업 목표
두 Workshop 패키지 내부에서 실제 개체 이동·충돌·지지 계산을 줄이는 구현이 있는지 확인한다. 기존 NPC 코드·PBO·실행 설정은 수정하지 않는다.

## 2. 실제 수행
공식 Steam 콘솔 workshop_download_item으로 Dark Horde(2007900691)를 받았다. manifest5347446939612704759, 코드 버전 V1.30.t23, PBO20,227,565bytes다. LGDynamicHorde(3690208839)는 Steam 항목별 응답 File Not Found(9)로 실패했다. 작업 종료의 No Error 문구와 항목별 실패를 혼동하지 않는다. LG 패키지를 얻었다거나 내부 코드를 검토했다고 보고하지 않는다.
REA open_binary는 PBO 형식 거부로 종료했다. 기존 공식 BankRev로 별도 연구 폴더에 추출해 scripts/**/*.c 전체를 토큰 검색하고 관련 함수와 설정을 직접 읽었다. 이 과정에서 새 분석 도구 설치·모드 구독·게임 실행은 하지 않았다. Steam 다운로드 캐시 변경은 발생했으나 모드 로드 목록은 변경하지 않았다.

## 3. 측정 결과
새 게임0회, 비용 측정0회. p50/p95/p99·물리 질의 실제 호출률·8/64 기능·화면 품질은 미측정이다. 타이머 값은 정적 예약 간격이며 실제 스케줄링 지연이나 엔진 갱신율이 아니다. PBO 복사본과 Steam 원본 SHA가 일치함을 확인했다. 전체 추출 파일 목록·크기·SHA와 주석 제외 API 후보를 별도 JSON으로 저장했다.

## 4. 원인 분석
### 실제 이동·충돌·지지 경로
- scripts/4_world/pvz_dh_4_world/dh_entities/pvz_dh_zombiebase.c:1은 ZombieBase extends DayZInfected를 수정한다. :187-269는 GetInputController와 OverrideMovementSpeed를 통한 속도 제한이다. 별도 CommandHandler나 SolveContacts 대체는 검색되지 않았다. 일반 감염자의 네이티브 기능을 그대로 물려받는 구조로, 엔진 내부 지지 검사 횟수나 캐시를 밝혀낸 것은 아니다.
- scripts/4_world/pvz_dh_4_world/dh_manage/pvz_dh_managehorde.c:607-640은 ECE_CREATEPHYSICS로 실제 감염자를 생성한다. :836-848의 1초 DarkFogMove/SetPosition은 호드 중심 엔티티의 이동이다. 모든 감염자를 1Hz 물리로 움직인다는 뜻이 아니다.
- scripts/3_game/pvz_dh_3_game/pvz_dh_raycastposition.c:7-22는 생성 위치 보정용 수직 RaycastRV다. 관절/발별 매 이동 솔버가 아니다. managezombies.c:435-462의 CheckUnderRoof RayCastBullet은 통째로 주석 처리되어 있다. 이를 활성 검사 비용으로 세지 않았다.
- managezombies.c:352-410은 순간이동 때 dBodyActive(INACTIVE),100ms 뒤 SetPosition,200ms 뒤 ACTIVE를 예약한다. 평상시 물리 LOD가 아니다. 지속적인 밀침/낙하 중 충돌 비활성화에 재사용하지 않는다.

### 계산량을 통제하는 실제 코드
| 방식 | 근거 | 적용 판단 |
|---|---|---|
| 플레이어 위치 공유 | dh_manage/pvz_dh_refreshplayerspositions.c:11-43, 기본1000ms 주기 공용 위치 목록 | 플레이어 열거·상위 목표 판단 중복 절감 후보. 각 개체 직접 인식이 제거됐다는 증거 아님 |
| 호드 단위 근접 확인 | dh_manage/pvz_dh_manageplayers.c:16,103-149,5000ms±500ms; 이탈 시 Master 삭제 | 활성 개체 수명 관리 참고. 일반 감염자 전원 즉시 삭제로 확대해석하지 않음 |
| 점진적 생성·정원 | managehorde.c:559-640; 정원·근접·중단 조건 통과 시 한 개체 생성 예약 | 생성 피크/개체 수 관리. 이미 활성64개체의 솔버 self-time 절감 아님 |
| 상태별 관리 간격 | zombiebase.c:192-269, 속도 관리100/1000/2000ms; constants.c:6의±50ms random | 타이머 분산은 피크 완화 후보. 연산 총량 감소나 최악 프레임 상한 보장은 없음 |
| 특성 변경 세대 검사 | zombiebase.c:275-288, 변경 번호가 다를 때만 특성 재적용 | 안전한 정적 설정 재계산 축소 참고. 물리 입력/결과 캐시와 다름 |
| 이탈·정체 회수 | managezombies.c:78-219,275-312,490-500 | 호드 중심에서 멀거나 정체하면 죽이거나 전송하고 삭제. 우리 정상 재등반 계약과 다름 |

### 실제 Master 역할 및 한계
managemaster.c CheckWhoToFollow는 Master의 mindState/위치를 읽어 followMaster와 호드 중심을 바꾼다. 전체 개체 경로를 하나로 풀어 배포하는 Flow Field나 개별 인식 OFF를 확인한 것은 아니다. Master가 있다는 설명만으로 공유 경로 최적화로 분류하지 않는다.
Ghost용 dz/ai/config.cpp와 dz/data/aiconfigs/config.cpp에는 ZombieWallPass 필터가 있고 disabled/crawl/crouch 등의 통행 허용 설정이 있다. 이는 기존 정상 벽·발판 판정을 유지하면서 비용만 줄이는 솔버가 아니다. 우리 안전 검사 대체로 선정하지 않는다.
동봉 MainOptions.xml의 Number_Of_Zombies는 Day20/Night15지만 DataHorde 정적 초기값은30/20, 생성 구조 템플릿은15/10으로 다르다. XML 읽기 코드가 값들을 갱신한다. 실제 서버 설정·실행 없이 어느 값이 최종 적용됐다고 단정하지 않는다. 어느 값도1000개체 근거리 성능의 증거가 아니다.

### 기존 연구와 중복 제거
기존 native-root-world-gate1/revision2 보고서에서 현재 커스텀 모델 Native Move가 컨테이너 벽을 통과해 직접 물리 대체가 기각됐다. Dark Horde가 DayZInfected를 사용한다는 이유로 이 시험을 반복하거나 AnimalBase NPC를 즉시 교체하지 않는다. 기존 Crowd 공간 격자·타이머/계획 예산·공유 플레이어 자료가 있으므로 동일 기법을 신규 구현 성과로 취급하지 않는다.

## 5. 검증 수준
Dark Horde 패키지 확보·추출·관련 Enforce 코드 정적 진단 성공. LG 패키지 확보 실패/내부 분석 미실행. 실행 기능·시각 품질·반복 성능·최종 수용은 전부 미실행이다. 성능 개선률은 제시하지 않는다.

## 6. 변경 사항
격리 연구 폴더의 원본 패키지 복사본·추출본·분석 스크립트·해시/근거·보고서와 체크포인트만 추가했다. 기존 게임 코드·PBO·임무·서버 설정 변경 없음. 신규 빌드/서명 수행 없음. 다운로드한 배포 서명은 보존했으며 이번 작업에서 검증 성공을 주장하지 않는다. 패키지·전체 제3자 소스는 공개 미러에 게시하지 않는다.

## 7. 원시 증거
실행 ID STATIC-HORDE-PACKAGE-20261114. acquisition.json,download-evidence.txt,extracted-manifest.json,static-api-candidates.json 및 고정 PBO와 추출 파일이 같은 연구 폴더에 있다. 스크립트 토큰 수는 정적 후보이며 실행 호출 수가 아니다. 원본 게임 실험·로그를 새 실행으로 기록하지 않는다.
공식 항목: https://steamcommunity.com/sharedfiles/filedetails/?id=2007900691 및 https://steamcommunity.com/sharedfiles/filedetails/?id=3690208839
Dark Horde PBO SHA256:2627d34f2f8acc718eb18994bc08f9b4320ea6e57551e63e5762a35176d80fef
직접 읽은 관련 함수·설정은 selected-evidence.json의 파일·행·해시로 연결한다. 기존 Source 사본과 현재 실행 PBO의 전체 동일성은 이번 검토에서 인증하지 않았다.

## 8. 잔여 문제
현재64/200 근거리 충돌 비용을 직접 줄일 이식 가능한 새 솔버는 발견하지 못했다. 재사용 우선순위는 (1) 기존 구현과 중복을 제거한 관리 작업의 변경 시 재계산/주기 분산, (2) 별도 대규모 원거리 활성 개체 수명 관리다. 둘 다 현재 물리 입력·안전 판정 생략과 분리한다. 정체 개체 죽이기/전송/벽 통과는 현재 지속 등반·밀침·낙하·재추적 보존 계약과 충돌하므로 제외한다.
LG 상세 코드 검토에는 제작자 또는 사용자가 정당하게 확보한 패키지가 추가로 필요하다. 공식 File Not Found를 회피하는 비공식 재배포본은 사용하지 않았다. 재다운로드 무한 반복·새 게임 시험·전체 엔진 분석으로 범위를 확대하지 않는다.
다음 단일 산출물은 기존 활성 NPC의 관리 작업별 현재 갱신율/변경 세대/소유자 표에서 아직 중복된 한 항목만 고르는 것이다. 직접 물리 비용과 관련 없는 미세 관리 비용만 확인되면 그 후보는 보류하고 기존 근거리 최적화로 복귀한다.
